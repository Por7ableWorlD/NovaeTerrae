// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTHealthComponent.h"
#include "Engine/World.h"
#include "Dev/Damage/LazerDamageType.h"
#include "Dev/Damage/NTDevBaseDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UNTHealthComponent::UNTHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNTHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    AActor* Owner = GetOwner();

    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UNTHealthComponent::OnTakeAnyDamage);
    }
}

void UNTHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld())
    {
        return;
    }

    // UE_LOG(LogHealthComponent, Display, TEXT("Current health before take damage: %.0f"), CurrentHealth);
    // UE_LOG(LogHealthComponent, Display, TEXT("Take damage: %.0f"), Damage);

    float RemainingDamage = Damage - CurrentShieldHealth;

    SetShieldHealth(CurrentShieldHealth - Damage);

    if (RemainingDamage > 0)
    {
        SetHealth(CurrentHealth - RemainingDamage);
    }    

    if (IsDead() && DamageCauser)
    {
        OnDeath.Broadcast(DamageCauser);
        UE_LOG(LogHealthComponent, Display, TEXT("Death\nLast damage: %.0f"), Damage);
    }
}

void UNTHealthComponent::RestoreFullHealth() 
{
    SetHealth(MaxHealth);
}

void UNTHealthComponent::SetHealth(float NewHealth)
{
    CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnCurrentHealthChanged.Broadcast(CurrentHealth);
}

void UNTHealthComponent::SetShieldHealth(float NewHealth) 
{
    CurrentShieldHealth = FMath::Clamp(NewHealth, 0.0f, MaxShieldHealth);
    OnCurrentHealthChanged.Broadcast(CurrentHealth);
}
