// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTHealthComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UNTHealthComponent::UNTHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    MaxHealth = 100;
    CurrentHealth = MaxHealth;
}

void UNTHealthComponent::RestoreHealth(float Healing)
{
    if (Healing <= 0)
    {
        return;
    }

    CurrentHealth = UKismetMathLibrary::Min(CurrentHealth + Healing, MaxHealth);
    OnHealthUpdate.Broadcast();
}

void UNTHealthComponent::TakeDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0)
    {
        return;
    }

    CurrentHealth = UKismetMathLibrary::Max(CurrentHealth - Damage, 0);
    OnHealthUpdate.Broadcast();

    if (CurrentHealth == 0)
    {
        OnActorDeath.Broadcast();
    }
}

void UNTHealthComponent::ChangeDefaultMaxHealth(float NewDefaultHealth)
{
    MaxHealth = NewDefaultHealth;
    CurrentHealth = MaxHealth;
}

// Called when the game starts
void UNTHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UNTHealthComponent::TakeDamage);
    }
}
