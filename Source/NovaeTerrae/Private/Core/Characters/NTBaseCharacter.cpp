// NOVAE TERRAE. All Rights Reserved.

#include "Core/Characters/NTBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/NTCharacterMovementComponent.h"

// Sets default values
ANTBaseCharacter::ANTBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UNTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ANTBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
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
        return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

    if (!Subsystem)
        return;

    Subsystem->AddMappingContext(InputMappingCpntext, 0);

    UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (PlayerInput)
    {
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Movement);
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &ANTBaseCharacter::OnStopMovement);

        PlayerInput->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::LookAround);

        PlayerInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ANTBaseCharacter::Jump);

        PlayerInput->BindAction(RunAction, ETriggerEvent::Started, this, &ANTBaseCharacter::OnStartRunnig);
        PlayerInput->BindAction(RunAction, ETriggerEvent::Completed, this, &ANTBaseCharacter::OnStopRunnig);
    }
}

void ANTBaseCharacter::Movement(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (!Controller)
        return;

    bIsMovingForward = InputVector.Y > 0.0f;

    AddMovementInput(GetActorForwardVector(), InputVector.Y);
    AddMovementInput(GetActorRightVector(), InputVector.X);
}

void ANTBaseCharacter::OnStopMovement()
{
    bIsMovingForward = false;
}

void ANTBaseCharacter::LookAround(const FInputActionValue& InputValue)
{
    FVector2D InputVector = InputValue.Get<FVector2D>();

    if (!Controller)
        return;

    APlayerController* PlayerController = Cast<APlayerController>(Controller);

    if (!PlayerController)
        return;

    PlayerController->SetControlRotation(
        FRotator(FMath::ClampAngle(PlayerController->GetControlRotation().Pitch + InputVector.Y * -1.0f, MinCameraAngle, MaxCameraAngle),
        PlayerController->GetControlRotation().Yaw + InputVector.X,
        PlayerController->GetControlRotation().Roll));
    

    //AddControllerYawInput(InputVector.X);
    //AddControllerPitchInput(InputVector.Y);
}

void ANTBaseCharacter::OnStartRunnig()
{
    bWantsToRun = true;
}

void ANTBaseCharacter::OnStopRunnig()
{
    bWantsToRun = false;
}

bool ANTBaseCharacter::IsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}