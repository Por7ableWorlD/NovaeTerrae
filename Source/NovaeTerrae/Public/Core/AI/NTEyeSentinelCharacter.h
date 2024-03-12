// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NTEyeSentinelCharacter.generated.h"

class UStaticMeshComponent;
class UNTEnemyHealthComponent;
class USplineComponent;
class UNiagaraSystem;

UCLASS()
class NOVAETERRAE_API ANTEyeSentinelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANTEyeSentinelCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTEnemyHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USplineComponent* SplineComponent;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem* DeathEffect;

protected:
	virtual void BeginPlay() override;

    void EnableAgressiveMode();

    void OnDeath(bool GetAbility = false);

    void OnStrafeEnable();

};
