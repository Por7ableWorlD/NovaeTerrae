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

    SetHealth(CurrentHealth - Damage);

    if (IsDead())
    {
        if (DamageType->IsA<ULazerDamageType>())
        {
            OnDeath.Broadcast(true);
        }
        else
        {
            OnDeath.Broadcast(false);
        }
        UE_LOG(LogHealthComponent, Display, TEXT("Death\nLast damage: %.0f"), Damage);
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

void UNTHealthComponent::SetHealth(float NewHealth)
{
    CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnCurrentHealthChanged.Broadcast(CurrentHealth);
}
