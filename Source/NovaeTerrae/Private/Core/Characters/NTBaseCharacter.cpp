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

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = false;
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

    Subsystem->AddMappingContext(InputMappingContext, 0);

    UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (PlayerInput)
    {
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Movement);
        PlayerInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &ANTBaseCharacter::OnStopMovement);

        PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANTBaseCharacter::Look);

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
        return;

    APlayerController* PlayerController = Cast<APlayerController>(Controller);

    if (!PlayerController)
        return;

#pragma region Rotation with camera
         double inRoll = PlayerController->GetControlRotation().Roll;
         double inPitch = PlayerController->GetControlRotation().Pitch;
         double inYaw = PlayerController->GetControlRotation().Yaw;

         inPitch = FMath::ClampAngle(inPitch + InputVector.Y * -1.0f, MinCameraAngle, MaxCameraAngle);

         PlayerController->SetControlRotation(FRotator(inPitch, inYaw + InputVector.X, inRoll));
#pragma endregion

    //double inPitch = PlayerController->GetControlRotation().Pitch;
    //inPitch = FMath::ClampAngle(inPitch + InputVector.Y * -1.0f, MinCameraAngle, MaxCameraAngle);

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