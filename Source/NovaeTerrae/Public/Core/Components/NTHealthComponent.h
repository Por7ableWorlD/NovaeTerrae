// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NTHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHealthChangedSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNTHealthComponent();

    FOnDeathSignature OnDeath;
    FOnCurrentHealthChangedSignature OnCurrentHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(CurrentHealth, 0.0f); }

    float GetCurrentHealth() const { return CurrentHealth; }

protected:
    virtual void BeginPlay() override;

    void SetHealth(float NewHealth);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

private:
    float CurrentHealth = 0.0f;

    UFUNCTION(BlueprintCallable)
    virtual void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
