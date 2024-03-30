// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/AI/NTBaseEnemyCharacter.h"
#include "NTDistortedHunter.generated.h"



UCLASS()
class NOVAETERRAE_API ANTDistortedHunter : public ANTBaseEnemyCharacter
{
	GENERATED_BODY()

public:
	ANTDistortedHunter();

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
    bool IsDead = false;

	virtual void BeginPlay() override;

    void EnableAgressiveMode(float Damage);

    UFUNCTION()
    void OnDeath(AActor* DeathCauser);

};
