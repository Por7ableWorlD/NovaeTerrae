// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTCompanionAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>

ANTCompanionAIController::ANTCompanionAIController()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void ANTCompanionAIController::BeginPlay()
{
    Super::BeginPlay();

    check(BehaviorTree);

    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    RunBehaviorTree(BehaviorTree.Get());
    BehaviorTreeComponent->StartTree(*BehaviorTree.Get());

    Following = true;
}

void ANTCompanionAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!Following && Player.Get()->GetVelocity().IsZero())
    {
        return;
    }

    const auto PawnLocation = GetPawn()->GetActorLocation();

    const auto DistanceToPlayer = FVector::Distance(Player.Get()->GetActorLocation(), PawnLocation);

    if (!Following && DistanceToPlayer < FollowActivationDistance)
    {
        return;
    }

    if (Following && DistanceToPlayer < FollowStopDistance)
    {
        Following = false;
        return;
    }
    
    Following = true;

    const float Acceleration = FMath::Clamp(
        (DistanceToPlayer - FollowStopDistance) / FollowActivationDistance, MinimumSpeedAcceleration, MaximumSpeedAcceleration);

    const float Speed = Acceleration * DeltaSeconds * BaseSpeed;

    GetPawn()->SetActorLocation(Speed * GetPawn()->GetActorForwardVector() + GetPawn()->GetActorLocation());

}

void ANTCompanionAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    check(Blackboard);
    check(BehaviorTree);

    Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset);
}