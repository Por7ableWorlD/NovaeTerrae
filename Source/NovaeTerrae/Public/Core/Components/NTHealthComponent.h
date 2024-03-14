// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NTHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, bool, GetAbility);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHealthChangedSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNTHealthComponent();
    
    UPROPERTY(BlueprintAssignable);
    FOnDeathSignature OnDeath;
    FOnCurrentHealthChangedSignature OnCurrentHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(CurrentHealth, 0.0f); }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }

    void SetHealth(float NewHealth);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UFUNCTION(BlueprintCallable)
    virtual void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual void RestoreFullHealth();

private:
    float CurrentHealth = 0.0f;
};
