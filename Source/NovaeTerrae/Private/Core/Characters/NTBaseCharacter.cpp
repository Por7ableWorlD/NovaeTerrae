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
#include "Core/AI/NTCompanionPawn.h"
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

    HealthComponentPrivet = CreateDefaultSubobject<UNTHealthComponent>("HealthComponent");

    ThirstComponent = CreateDefaultSubobject<UNTThirstComponent>("ThirstComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    ThirstTextComponent = CreateDefaultSubobject<UTextRenderComponent>("ThirstTextComponent");
    ThirstTextComponent->SetupAttachment(GetRootComponent());
}

void ANTBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponentPrivet);
    check(HealthTextComponent);
    check(ThirstTextComponent);
    check(GetCharacterMovement());

    OnCurrentHealthChanged(HealthComponentPrivet->GetCurrentHealth());
    HealthComponentPrivet->OnCurrentHealthChanged.AddUObject(this, &ANTBaseCharacter::OnCurrentHealthChanged);
    HealthComponentPrivet->OnDeath.AddUObject(this, &ANTBaseCharacter::OnDeath);

    OnCurrentThirstChanged(ThirstComponent->GetCurrentThirst());
    ThirstComponent->OnCurrentThirstChanged.AddUObject(this, &ANTBaseCharacter::OnCurrentThirstChanged);

    LandedDelegate.AddDynamic(this, &ANTBaseCharacter::OnGroundLanded);

    Companion = Cast<ANTCompanionPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ANTCompanionPawn::StaticClass()));
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

    Subsystem->AddMappingContext(InputMappingContext, 0);

    UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (PlayerInput)
    {
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Movement);
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &ANTBaseCharacter::OnStopMovement);

        PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Look);

        PlayerInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ANTBaseCharacter::Jump);

        PlayerInput->BindAction(DashAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnDash);

        PlayerInput->BindAction(RunAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnStartRunnig);
        PlayerInput->BindAction(RunAction, ETriggerEvent::Completed, this, &ANTBaseCharacter::OnStopRunnig);
    }
}

void ANTBaseCharacter::Movement(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (!Controller)
    {
        return;
    }

    bIsMovingForward = InputVector.Y > 0.0f;

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

void ANTBaseCharacter::OnStopMovement()
{
    bIsMovingForward = false;
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

void ANTBaseCharacter::OnResetDeath()
{
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    EnableInput(PlayerController);
    OnResetDeathSignature.Broadcast();
    HealthComponentPrivet->SetHealth(HealthComponentPrivet->GetMaxHealth());

    AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
    if (!PlayerStart)
    {
        UE_LOG(LogBaseCharacter, Warning, TEXT("[Warning] Can't find player's start object!"));
    }

    Companion->TeleportTo(PlayerStart->GetActorLocation() + FVector(100.0f, 0.0f, 0.0f), PlayerStart->GetActorRotation());
    this->TeleportTo(PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());  
    
}

bool ANTBaseCharacter::IsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

void ANTBaseCharacter::OnCurrentHealthChanged(float CurrentHealth)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
}

void ANTBaseCharacter::OnCurrentThirstChanged(float CurrentThirst)
{
    ThirstTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentThirst)));
}

void ANTBaseCharacter::OnDeath(bool IsLazer)
{
    DeathAnimMontage->bEnableAutoBlendOut = true;
    PlayAnimMontage(DeathAnimMontage);

    //GetCharacterMovement()->DisableMovement();
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    DisableInput(PlayerController);

    GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ANTBaseCharacter::OnResetDeath, 5, false);

    ThirstComponent->SetThirst(FMath::Clamp(ThirstComponent->GetCurrentThirst() + 1, 0, ThirstComponent->MaxThirst));

    //SetLifeSpan(LifeSpanOnDeath);

    OnDeathSignature.Broadcast(IsLazer);

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

    TakeDamage(HealthComponentPrivet->GetCurrentHealth(), FDamageEvent{}, nullptr, nullptr);
}

void ANTBaseCharacter::BitCompanion()
{
    ThirstComponent->SetThirst(0);
    UGameplayStatics::ApplyDamage(Companion, 25.0f, Controller, Companion, nullptr);
}