// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NTHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHealthChangedSignature, float);
DECLARE_MULTICAST_DELEGATE(FOnPlayerDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNTHealthComponent();

    FOnDeathSignature OnDeath;
    FOnCurrentHealthChangedSignature OnCurrentHealthChanged;
    FOnPlayerDeath OnPlayerDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(CurrentHealth, 0.0f); }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetDefaultMaxHealth(float NewMaxHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void RestoreFullHealth();

    void SetHealth(float NewHealth);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

private:
    float CurrentHealth = 0.0f;
    bool SetDefaultMaxHealthUsed = false;

    UFUNCTION(BlueprintCallable)
    virtual void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
