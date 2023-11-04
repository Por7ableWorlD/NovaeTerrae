// NOVAE TERRAE. All Rights Reserved.

#include "Core/Characters/NTBaseCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
ANTBaseCharacter::ANTBaseCharacter()
{
    // Set this character to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(GetRootComponent());
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
}
