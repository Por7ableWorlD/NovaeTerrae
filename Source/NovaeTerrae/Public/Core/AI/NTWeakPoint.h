// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NTWeakPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeakPointDestroyed);

class UNTEnemyHealthComponent;
class UNiagaraSystem;

UCLASS()
class NOVAETERRAE_API ANTWeakPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ANTWeakPoint();

	UPROPERTY(BlueprintAssignable);
    FOnWeakPointDestroyed OnWeakPointDestroyed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTEnemyHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem* DeathEffect;

protected:
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "WeakPoint")
    void SetParameters(float WeakPointHealth, float DamageResistance, float DeathAdditionalDamage);

	UFUNCTION()
    void OnDeath(bool GetAbility = false);

private:
    void TransferDamage(float Damage);

	float AdditionalDamage = 10.0f;
    AActor* OwnerRef;
};
