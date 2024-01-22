// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EyeSentinelBTTask_Strafe.generated.h"


UCLASS()
class NOVAETERRAE_API UEyeSentinelBTTask_Strafe : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UEyeSentinelBTTask_Strafe();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;

    const TArray<float> RandomStrafeModifier = TArray<float>{-4.0f, -3.0f, 3.0f, 4.0f};
};
