// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/AI/NTBaseEnemyCharacter.h"
#include "NTRocketSentinelCharacter.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class NOVAETERRAE_API ANTRocketSentinelCharacter : public ANTBaseEnemyCharacter
{
	GENERATED_BODY()

public:
	ANTRocketSentinelCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UNiagaraSystem* DeathEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Behavior")
    float ShieldDuration = 5.0f;

protected:
    FTimerHandle ShieldTimerHandle;

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnDeath(AActor* DeathCauser);

    void OnShieldEnable();

    void OnShieldDisable();
};
