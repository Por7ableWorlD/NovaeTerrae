// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTCharacterMovementComponent.h"
#include "Core/Characters/NTBaseCharacter.h"

float UNTCharacterMovementComponent::GetMaxSpeed() const
{
    float MaxSpeed = Super::GetMaxSpeed();
    const ANTBaseCharacter* Player = Cast<ANTBaseCharacter>(GetPawnOwner());

    if (!Player)
    {
        return MaxSpeed;
    }

    if (Player->IsWalking())
    {
        MaxSpeed *= WalkModifier;
    }

    if (Player->IsAiming())
    {
        MaxSpeed *= AimModifier;
    }

    return MaxSpeed;
}
