// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTThirstComponent.h"

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
}
