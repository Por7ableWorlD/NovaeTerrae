// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/AI/NTBaseEnemyCharacter.h"
#include "NTEyeSentinelCharacter.generated.h"

class USplineComponent;
class UNiagaraSystem;

UCLASS()
class NOVAETERRAE_API ANTEyeSentinelCharacter : public ANTBaseEnemyCharacter
{
	GENERATED_BODY()

public:
	ANTEyeSentinelCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USplineComponent* SplineComponent;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem* DeathEffect;

protected:
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnDeath(AActor* DeathCauser);

    void OnStrafeEnable() const;

};
