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

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnDeath(AActor* DeathCauser);

};
