// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/Tasks/EyeSentinelBTTask_Strafe.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogEyeSentinelBTTaskStrafe, All, All)

UEyeSentinelBTTask_Strafe::UEyeSentinelBTTask_Strafe() 
{
    NodeName = TEXT("Eye Sentinel Strafe");

    BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UEyeSentinelBTTask_Strafe, BlackboardKey));
}

EBTNodeResult::Type UEyeSentinelBTTask_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController {OwnerComp.GetAIOwner()};

    check(AIController);

    APawn* Pawn {AIController->GetPawn()};

    FVector ActorOrigin, BoxExtent;
    Pawn->GetActorBounds(false, ActorOrigin, BoxExtent, false);

    const FVector ActorLocation {Pawn->GetActorLocation()};
    const FVector NewLocation = BoxExtent * RandomStrafeModifier[FMath::RandRange(0, 3)] * Pawn->GetActorRightVector() + ActorLocation;

    const UNavigationSystemV1* NavSystem{UNavigationSystemV1::GetCurrent(GetWorld())};

    check(NavSystem);

    UNavigationPath* path = NavSystem->FindPathToLocationSynchronously(this, ActorLocation, NewLocation, Pawn);

    check(path);

    AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKey.SelectedKeyName, false);

    FAIMoveRequest MoveRequest;
    MoveRequest.SetAcceptanceRadius(30.0f);
    MoveRequest.SetUsePathfinding(true);
    AIController->RequestMove(MoveRequest, path->GetPath());

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}

FString UEyeSentinelBTTask_Strafe::GetStaticDescription() const
{
    return FString::Printf(TEXT("Bool: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
