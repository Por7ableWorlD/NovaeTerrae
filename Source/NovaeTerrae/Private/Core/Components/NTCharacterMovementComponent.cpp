// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTCharacterMovementComponent.h"
#include "Core/Characters/NTBaseCharacter.h"

float UNTCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ANTBaseCharacter* Player = Cast<ANTBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
