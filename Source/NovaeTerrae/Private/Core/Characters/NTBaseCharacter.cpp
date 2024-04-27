// NOVAE TERRAE. All Rights Reserved.

#include "Core/Characters/NTBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/NTCharacterMovementComponent.h"
#include "Components/NTHealthComponent.h"
#include "Components/NTThirstComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "Core/AI/NTCompanionCharacter.h"
#include <Core/Dev/GameplayTags/StatusGameplayTags.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

ANTBaseCharacter::ANTBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UNTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    GetCharacterMovement()->bUseSeparateBrakingFriction = false;
    GetCharacterMovement()->FallingLateralFriction = 5.0f;

    JumpMaxCount = 2;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UNTHealthComponent>("HealthComponent");

    ThirstComponent = CreateDefaultSubobject<UNTThirstComponent>("ThirstComponent");
}

void ANTBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(GetCharacterMovement());

    HealthComponent->OnDeath.AddDynamic(this, &ANTBaseCharacter::OnDeath);

    LandedDelegate.AddDynamic(this, &ANTBaseCharacter::OnGroundLanded);

    Companion = Cast<ANTCompanionCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANTCompanionCharacter::StaticClass()));

    if (!Companion)
    {
        return;
    }

    Companion->OnSacrificeStart.AddUObject(this, &ANTBaseCharacter::SacrificingHeal);
}

void ANTBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANTBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    APlayerController* PlayerController = Cast<APlayerController>(Controller);

    if (!PlayerController)
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

    if (!Subsystem)
    {
        return;
    }

    Subsystem->AddMappingContext(LocomotionInputMappingContext, 0);
    Subsystem->AddMappingContext(ExplorationInputMappingContext, 0);
    Subsystem->AddMappingContext(DefaultInputMappingContext, 0);

    UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (PlayerInput)
    {
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Movement);

        PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Look);

        PlayerInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ANTBaseCharacter::Jump);

        PlayerInput->BindAction(DashAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnDash);

        PlayerInput->BindAction(RunAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnStartRunnig);
        PlayerInput->BindAction(RunAction, ETriggerEvent::Completed, this, &ANTBaseCharacter::OnStopRunnig);

        PlayerInput->BindAction(ThirstRemoveAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnThirstRemove);

        PlayerInput->BindAction(SacrificeAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnSacrifice);

        PlayerInput->BindAction(FastReloadAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnFastReload);

        PlayerInput->BindAction(ScanAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnScan);
    }
}

void ANTBaseCharacter::Movement(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (!Controller)
    {
        return;
    }

    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    // get right vector
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, InputVector.Y);
    AddMovementInput(RightDirection, InputVector.X);
}

void ANTBaseCharacter::Look(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (!Controller)
    {
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(Controller);

    if (!PlayerController)
    {
        return;
    }

    double inRoll = PlayerController->GetControlRotation().Roll;
    double inPitch = PlayerController->GetControlRotation().Pitch;
    double inYaw = PlayerController->GetControlRotation().Yaw;

    inPitch = FMath::ClampAngle(inPitch + InputVector.Y * -1.0f, MinCameraAngle, MaxCameraAngle);

    PlayerController->SetControlRotation(FRotator(inPitch, inYaw + InputVector.X, inRoll));

    // AddControllerYawInput(InputVector.X);
    // AddControllerPitchInput(InputVector.Y);
}

void ANTBaseCharacter::OnStartRunnig()
{
    bWantsToRun = true;
}

void ANTBaseCharacter::OnStopRunnig()
{
    bWantsToRun = false;
}

void ANTBaseCharacter::OnDash() 
{
    if ((IsRunning() && !GetCharacterMovement()->IsFalling()) || (!CanDash))
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(DashReseter, this, &ANTBaseCharacter::OnResetDash, ResetDashDelay, false);

    LaunchCharacter(FVector{GetVelocity().X * 10.0f, GetVelocity().Y * 10.0f, 0}, false, false);
    CanDash = false;
}

void ANTBaseCharacter::OnResetDash() 
{
    CanDash = true;
    GetWorld()->GetTimerManager().ClearTimer(DashReseter);

   // UE_LOG(LogBaseCharacter, Error, TEXT("Heal: %f"), HealthComponent->GetMaxHealth());
}

void ANTBaseCharacter::OnThirstRemove() 
{
    if (!GameTags.HasTag(FStatusGameplayTags::Get().Thirst))
    {
        return;
    }

    ThirstComponent->SetThirst(0.0f);
    GameTags.RemoveTag(FStatusGameplayTags::Get().Thirst);
    HealthComponent->SetHealth(HealthComponent->GetCurrentHealth() + 25.0f);
    OnThirstRemoveSignature.Broadcast();
}

void ANTBaseCharacter::OnSacrifice() 
{
    if (HealthComponent->GetCurrentHealth() == HealthComponent->GetMaxHealth())
    {
        return;
    }

    OnSacrificeRequestSignature.Broadcast();
}

void ANTBaseCharacter::SacrificingHeal(float SacrificedHealth)
{
    float NewHealth = HealthComponent->GetCurrentHealth() + SacrificedHealth;
    HealthComponent->SetHealth(NewHealth);
}

void ANTBaseCharacter::OnScan()
{
    OnScanRequestSignature.Broadcast();
}

void ANTBaseCharacter::OnFastReload()
{
    OnFastReloadRequestSignature.Broadcast();
}

void ANTBaseCharacter::OnResetDeath()
{
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    EnableInput(PlayerController);
    HealthComponent->SetHealth(HealthComponent->GetMaxHealth());
    OnResetPlayerDeathSignature.Broadcast();
}

bool ANTBaseCharacter::IsRunning() const
{
    return bWantsToRun && !GetVelocity().IsZero();
}

void ANTBaseCharacter::OnDeath(AActor* DeathCauser)
{
    ThirstComponent->SetThirst(ThirstComponent->GetCurrentThirst() + 1);

    if (!GameTags.HasTagExact(FStatusGameplayTags::Get().Thirst))
    {
        PlayDeathAnimation(DeathCauser, false);
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(
        DeathTimer, [&, DeathCauser] {
            if (GameTags.HasTagExact(FStatusGameplayTags::Get().Thirst))
            {
                PlayDeathAnimation(DeathCauser, true);
            }
        },
        ThirstComponent->GetThirstDuration(), false);
    
}

void ANTBaseCharacter::PlayDeathAnimation(AActor* DeathCauser, bool ResetThirst)
{
    if (ResetThirst)
    {
        ThirstComponent->SetThirst(0.0f);
        GameTags.RemoveTag(FStatusGameplayTags::Get().Thirst);
    }

    DeathAnimMontage->bEnableAutoBlendOut = true;
    PlayAnimMontage(DeathAnimMontage);

    // GetCharacterMovement()->DisableMovement();
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    DisableInput(PlayerController);

    GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ANTBaseCharacter::OnResetDeath, 5, false);

    // SetLifeSpan(LifeSpanOnDeath);

    OnPlayerDeathSignature.Broadcast(DeathCauser);

    UE_LOG(LogBaseCharacter, Display, TEXT("OnDeath event. Play montage Death"));
}

void ANTBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;

    UE_LOG(LogBaseCharacter, Display, TEXT("Fall Velocity Z = %.0f"), FallVelocityZ);

    if (FallVelocityZ < LandedDeathVelocity)
    {
        return;
    }

    TakeDamage(HealthComponent->GetCurrentHealth(), FDamageEvent{}, nullptr, nullptr);
}