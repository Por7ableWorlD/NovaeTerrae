// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NTEyeSentinelCharacter.generated.h"

class UC3_LAA_MainComponent;
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
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UC3_LAA_MainComponent* LAAComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTEnemyHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USplineComponent* SplineComponent;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior", meta = (ClampMin = "100.0", ClampMax = "5000.0"))
    float AttackRadius = 3000.0f;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem* DeathEffect;

protected:
	virtual void BeginPlay() override;

    void EnableAgressiveMode();

    void OnDeath(bool GetAbility = false);

    void OnStrafeEnable();

};
