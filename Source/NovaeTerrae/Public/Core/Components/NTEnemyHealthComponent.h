// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Components/NTHealthComponent.h"
#include "GameplayTagContainer.h"
#include "NTEnemyHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTakeDamageFromEnemy);
DECLARE_MULTICAST_DELEGATE(FOnActionThresholdReached);

class AAIController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTEnemyHealthComponent : public UNTHealthComponent
{
	GENERATED_BODY()

public:
    UNTEnemyHealthComponent();

    FOnActionThresholdReached OnActionThresholdReached;
    FOnTakeDamageFromEnemy OnTakeDamageFromPlayer;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior", meta = (ClampMin = "0.1", ClampMax = "100.0"))
    float ActionThresholdPercentage = 25.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior")
    bool EnableActionThreshold = true;

    void OnTakeDamageFromEnemy(AAIController* AIController);

    void CheckActionThreshold();

    FGameplayTagContainer GameTags;

protected:
    virtual void BeginPlay() override;
    virtual void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetDefaultMaxHealth(float NewMaxHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void RestoreFullHealth();

private:
    bool SetDefaultMaxHealthUsed = false;
    float ThresholdValue;
    int ThresholdNumber = 1;
};
