// NOVAE TERRAE. All Rights Reserved.


#include "Core/Components/NTEnemyHealthComponent.h"
#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Core/Characters/NTBaseCharacter.h"
#include <AIController.h>
#include "Core/AI/NTWeakPoint.h"
#include <Core/Dev/GameplayTags/StatusGameplayTags.h>
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemyHealthComponent, All, All)

UNTEnemyHealthComponent::UNTEnemyHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNTEnemyHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    ThresholdValue = GetMaxHealth() * (1 - (ActionThresholdPercentage * ThresholdNumber) / 100);
}

void UNTEnemyHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (!DamageCauser || (!DamageCauser->IsA<ANTBaseCharacter>() && !DamageCauser->IsA<ANTWeakPoint>()))
    {
        return;
    }

    if (GameTags.HasTag(FStatusGameplayTags::Get().Invulnerability))
    {
        return;
    } 

    if (DamageCauser->IsA<ANTBaseCharacter>())
    {
        float FinalDamage = Damage * (1 - (DamageResistancePercentage / 100));
        OnTakeDamageFromPlayer.Broadcast(FinalDamage);
        Super::OnTakeAnyDamage(DamagedActor, FinalDamage, DamageType, InstigatedBy, DamageCauser);
    }

    if (DamageCauser->IsA<ANTWeakPoint>())
    {
        OnTakeDamageFromPlayer.Broadcast(Damage);
        Super::OnTakeAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
    }

}

void UNTEnemyHealthComponent::OnTakeDamageFromEnemy(AAIController* AIController) 
{
    check(AIController);

    AIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    AIController->GetBlackboardComponent()->SetValueAsBool(FName("AgressiveState"), true);
    
    CheckActionThreshold();
}

void UNTEnemyHealthComponent::CheckActionThreshold()
{
    if (!EnableActionThreshold)
    {
        return;
    }

    float Health = GetCurrentHealth();

    UE_LOG(LogEnemyHealthComponent, Display, TEXT("[DEBUG] Current Health: %.0f"), Health);
    UE_LOG(LogEnemyHealthComponent, Display, TEXT("[DEBUG] Current Threshold: %.0f"), ThresholdValue);

    if (Health > ThresholdValue)
    {
        return;
    }

    OnActionThresholdReached.Broadcast();

    float NewThreshold = GetMaxHealth() * (1 - (ActionThresholdPercentage * (ThresholdNumber + 1)) / 100);

    if (NewThreshold < 0)
    {
        return;
    }

    UE_LOG(LogEnemyHealthComponent, Display, TEXT("[DEBUG] Old Threshold: %.0f"), ThresholdValue);

    ThresholdValue = NewThreshold;
    ++ThresholdNumber;

    UE_LOG(LogEnemyHealthComponent, Display, TEXT("[DEBUG] Old Threshold: %.0f"), ThresholdValue);
}

void UNTEnemyHealthComponent::SetDefaultMaxHealth(float NewMaxHealth)
{
    if (SetDefaultMaxHealthUsed)
    {
        return;
    }

    MaxHealth = NewMaxHealth;
    SetDefaultMaxHealthUsed = true;
    RestoreFullHealth();
}

void UNTEnemyHealthComponent::RestoreFullHealth()
{
    if (EnableActionThreshold)
    {
        ThresholdNumber = 1;
        ThresholdValue = GetMaxHealth() * (1 - (ActionThresholdPercentage * ThresholdNumber) / 100);
    }

    SetHealth(MaxHealth);
}