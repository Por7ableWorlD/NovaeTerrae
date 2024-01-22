// NOVAE TERRAE. All Rights Reserved.

#include "Core/Components/NTCompanionHealthComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Core/Characters/NTBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogCompanionHealthComponent, All, All)

UNTCompanionHealthComponent::UNTCompanionHealthComponent() {}

void UNTCompanionHealthComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNTCompanionHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || !GetWorld() || HealTimerHandle.IsValid() || IsDead() || !DamageCauser->IsA<ANTBaseCharacter>())
    {
        return;
    }

    // GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    SetHealth(GetCurrentHealth() - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
        UE_LOG(LogCompanionHealthComponent, Display, TEXT("Death\nLast damage: %.0f"), Damage);
    }

    if (AutoHeal && (FMath::IsNearlyEqual(GetCurrentHealth(), AutoHealStartLimit) || (GetCurrentHealth() < AutoHealStartLimit)))
    {
        GetWorld()->GetTimerManager().SetTimer(
            HealTimerHandle, this, &UNTCompanionHealthComponent::HealUpdate, HealUpdateTime, true, StartHealDelay);
    }
}

void UNTCompanionHealthComponent::HealUpdate()
{
    SetHealth(GetCurrentHealth() + HealModifier);

    if (FMath::IsNearlyEqual(GetCurrentHealth(), MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
        UE_LOG(LogCompanionHealthComponent, Display, TEXT("Auto heal stoped. Healed Max. CurrentHealth = %.0f"), GetCurrentHealth());
    }
}
