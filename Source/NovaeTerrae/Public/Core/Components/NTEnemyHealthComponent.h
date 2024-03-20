// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Components/NTHealthComponent.h"
#include "GameplayTagContainer.h"
#include "NTEnemyHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamageFromPlayer, float);
DECLARE_MULTICAST_DELEGATE(FOnActionThresholdReached);

class AAIController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTEnemyHealthComponent : public UNTHealthComponent
{
	GENERATED_BODY()

public:
    UNTEnemyHealthComponent();

    FOnActionThresholdReached OnActionThresholdReached;
    FOnTakeDamageFromPlayer OnTakeDamageFromPlayer;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior")
    bool EnableActionThreshold = true;

    void OnTakeDamageFromEnemy(AAIController* AIController);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetDefaultMaxHealth(float NewMaxHealth);

    void SetDamageResistancePercentage(float DamageResistance)
    {
        DamageResistancePercentage = FMath::Clamp(DamageResistance, 0.0f, 100.0f);
    };

    float GetDamageResistancePercentage() const { return DamageResistancePercentage; };

    FGameplayTagContainer GameTags;

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior",
        meta = (EditCondition = "EnableActionThreshold", ClampMin = "0.1", ClampMax = "100.0"))
    float ActionThresholdPercentage = 25.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float DamageResistancePercentage = 0.0f;

    virtual void BeginPlay() override;
    virtual void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

    virtual void RestoreFullHealth() override;

private:
    bool SetDefaultMaxHealthUsed = false;
    float ThresholdValue;
    int ThresholdNumber = 1;

    void CheckActionThreshold();
};
