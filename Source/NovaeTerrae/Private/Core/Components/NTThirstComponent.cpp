// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTThirstComponent.h"
#include <Core/Dev/GameplayTags/StatusGameplayTags.h>
#include "Core/Characters/NTBaseCharacter.h"

UNTThirstComponent::UNTThirstComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNTThirstComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNTThirstComponent::SetThirst(float NewThirst) 
{
    CurrentThirst = FMath::Clamp(NewThirst, 0.0f, MaxThirst);
    OnCurrentThirstChanged.Broadcast(CurrentThirst);

    if (CurrentThirst != MaxThirst)
    {
        return;
    }

    ANTBaseCharacter* Player = Cast<ANTBaseCharacter>(GetOwner());

    if (!Player)
    {
        return;
    }

    Player->GameTags.AddTag(FStatusGameplayTags::Get().Thirst);
}
