// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NTCompanionPawn.generated.h"

class USphereComponent;
class UC3_LAA_MainComponent;

UCLASS()
class NOVAETERRAE_API ANTCompanionPawn : public APawn
{
	GENERATED_BODY()

public:
	ANTCompanionPawn();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
    TObjectPtr<USphereComponent> SphereCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LAA")
    TObjectPtr<UC3_LAA_MainComponent> LAAComponent;

protected:
	virtual void BeginPlay() override;

};
