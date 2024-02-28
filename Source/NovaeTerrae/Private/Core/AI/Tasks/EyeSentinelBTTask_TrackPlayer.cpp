// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/Tasks/EyeSentinelBTTask_TrackPlayer.h"
#include "Core/AI/NTCompanionCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Containers/Array.h"
#include "CollisionQueryParams.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogEyeSentinelBTTaskTrackPlayer, All, All)

UEyeSentinelBTTask_TrackPlayer::UEyeSentinelBTTask_TrackPlayer()
{
    bCreateNodeInstance = true;

    NodeName = TEXT("Eye Sentinel Track Player");

    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEyeSentinelBTTask_TrackPlayer, BlackboardKey));
}

EBTNodeResult::Type UEyeSentinelBTTask_TrackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIController = OwnerComp.GetAIOwner();

    check(AIController);

    Pawn = AIController->GetPawn();

    if (ActorsToIgnore.IsEmpty()) {
        ActorsToIgnore.Add(Pawn);
        ActorsToIgnore.Add(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        ActorsToIgnore.Add(UGameplayStatics::GetActorOfClass(GetWorld(), ANTCompanionCharacter::StaticClass()));
    }

    UE_LOG(LogEyeSentinelBTTaskTrackPlayer, Display, TEXT("[DEBUG] Before Time: %f"), CurrentTime);
    CurrentTime = 0.0f;
    UE_LOG(LogEyeSentinelBTTaskTrackPlayer, Display, TEXT("[DEBUG] After Time: %f"), CurrentTime);

    GetWorld()->GetTimerManager().SetTimer(TrackTimerHandle, this, &UEyeSentinelBTTask_TrackPlayer::UpdateMarkLocation, TrackTick, true);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;

}

FString UEyeSentinelBTTask_TrackPlayer::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

void UEyeSentinelBTTask_TrackPlayer::UpdateMarkLocation()
{
    FHitResult HitResult;

    FVector TraceStart = Pawn->GetActorLocation();
    FVector TraceEnd = TraceStart + Pawn->GetActorForwardVector() * 3000.0f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActors(ActorsToIgnore);

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);

    DrawDebugCylinder(GetWorld(), HitResult.Location, HitResult.Location, 100.0f, 12, FColor::Red, false, TrackTick, 0, 2.0f);

    AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, HitResult.Location);

    CurrentTime += TrackTick;
    // UE_LOG(LogEyeSentinelBTTaskTrackPlayer, Display, TEXT("[DEBUG] Current Time: %f"), CurrentTime);
    // UE_LOG(LogEyeSentinelBTTaskTrackPlayer, Display, TEXT("[DEBUG] Tracking Time: %f"), TrackingTime);

    if (TrackingTime <= CurrentTime)
    {
        UE_LOG(LogEyeSentinelBTTaskTrackPlayer, Display, TEXT("[DEBUG] Clear timer"));
        GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);
    }
}
