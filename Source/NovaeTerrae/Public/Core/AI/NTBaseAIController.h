// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NTBaseAIController.generated.h"

UCLASS()
class NOVAETERRAE_API ANTBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
    void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
    
    UFUNCTION(BlueprintImplementableEvent, Category = "AIPerception")
    void DetermineAISightPerceptionViewPoint(FVector& OutLocation, FRotator& OutRotation) const;
};
