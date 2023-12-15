// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NTCompanionAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class NOVAETERRAE_API ANTCompanionAIController : public AAIController
{
	GENERATED_BODY()

public:
    ANTCompanionAIController();

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Follow")
    float BaseSpeed = 300.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Follow")
    float FollowActivationDistance = 400.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Follow")
    float FollowStopDistance = 150.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Follow")
    float MinimumSpeedAcceleration = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Follow")
    float MaximumSpeedAcceleration = 3.0f;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void OnPossess(APawn* InPawn) override;

private:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    TObjectPtr<UBehaviorTree> BehaviorTree;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
    TObjectPtr<UBlackboardComponent> BlackboardComponent;

    TObjectPtr<ACharacter> Player;

    bool Following;
};

