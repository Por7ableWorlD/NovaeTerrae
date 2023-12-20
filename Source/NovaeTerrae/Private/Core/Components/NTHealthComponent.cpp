// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTHealthComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
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

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    // UE_LOG(LogHealthComponent, Display, TEXT("Current health before take damage: %.0f"), CurrentHealth);
    // UE_LOG(LogHealthComponent, Display, TEXT("Take damage: %.0f"), Damage);

    SetHealth(CurrentHealth - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
        UE_LOG(LogHealthComponent, Display, TEXT("Death\nLast damage: %.0f"), Damage);
    }
    else if (AutoHeal && (FMath::IsNearlyEqual(CurrentHealth, AutoHealStartLimit) || (CurrentHealth < AutoHealStartLimit)))
    {
        GetWorld()->GetTimerManager().SetTimer(
            HealTimerHandle, this, &UNTHealthComponent::HealUpdate, HealUpdateTime, true, StartHealDelay);
    }

    // Print For Debug Damage Type
    if (DamageType)
    {
        if (DamageType->IsA<UNTDevBaseDamageType>())
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "DevBaseDamage");
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, "None");
        }
    }
}

void UNTHealthComponent::HealUpdate()
{
    SetHealth(CurrentHealth + HealModifier);

    if (FMath::IsNearlyEqual(CurrentHealth, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
        UE_LOG(LogHealthComponent, Display, TEXT("Auto heal stoped. Healed Max. CurrentHealth = %.0f"), CurrentHealth);
    }
}

void UNTHealthComponent::SetHealth(float NewHealth)
{
    CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnCurrentHealthChanged.Broadcast(CurrentHealth);
}
