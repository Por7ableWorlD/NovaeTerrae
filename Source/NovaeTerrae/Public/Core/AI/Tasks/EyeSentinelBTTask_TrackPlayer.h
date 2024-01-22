// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EyeSentinelBTTask_TrackPlayer.generated.h"


UCLASS()
class NOVAETERRAE_API UEyeSentinelBTTask_TrackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UEyeSentinelBTTask_TrackPlayer();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behavior", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float TrackingTime = 2.0f;

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;

    FTimerHandle TrackTimerHandle;
    FTimerHandle ExecutionAbortTimerHandle;
    TArray<AActor*> ActorsToIgnore;
    AAIController* AIController;
    APawn* Pawn;
    float CurrentTime;
    float TrackTick = 0.025f;

    void UpdateMarkLocation();
};
