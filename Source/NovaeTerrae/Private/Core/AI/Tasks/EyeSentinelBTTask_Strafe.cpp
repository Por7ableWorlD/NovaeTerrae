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

    // FVector ActorOrigin, BoxExtent;
    // Pawn->GetActorBounds(true, ActorOrigin, BoxExtent, false);

    // DrawDebugBox(GetWorld(), ActorOrigin, BoxExtent, FColor::Black, false, 2.0f);

    const FVector ActorLocation {Pawn->GetActorLocation()};
    const FVector NewLocation = 150 * RandomStrafeModifier[FMath::RandRange(0, 3)] * Pawn->GetActorRightVector() + ActorLocation;

    const UNavigationSystemV1* NavSystem{UNavigationSystemV1::GetCurrent(GetWorld())};

    if (!ensureMsgf(NavSystem, TEXT("[Warning] Navigation system not found or isn't available")))
    {
        return EBTNodeResult::Failed;
    }

    if (!ensureMsgf(NavSystem->GetDefaultNavDataInstance(), TEXT("[Warning] Navigation mesh not found on the current level")))
    {
        return EBTNodeResult::Failed;
    }
    
    UNavigationPath* path = NavSystem->FindPathToLocationSynchronously(this, ActorLocation, NewLocation, Pawn);
    
    if (!path)
    {
        UE_LOG(LogEyeSentinelBTTaskStrafe, Display, TEXT("[Debug] Strafe task couldn't find path for strafe to new location"));
        return EBTNodeResult::Failed;
    }

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
