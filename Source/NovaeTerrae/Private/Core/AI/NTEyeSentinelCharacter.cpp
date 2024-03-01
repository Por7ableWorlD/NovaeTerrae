// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTEyeSentinelCharacter.h"
#include "Core/Components/NTEnemyHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include <C3_LAA_MainComponent.h>
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SplineComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <AIController.h>

DEFINE_LOG_CATEGORY_STATIC(LogEyeSentinel, All, All)

ANTEyeSentinelCharacter::ANTEyeSentinelCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    LAAComponent = CreateDefaultSubobject<UC3_LAA_MainComponent>("LAAComponent");

    HealthComponent = CreateDefaultSubobject<UNTEnemyHealthComponent>("HealthComponent");

    SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void ANTEyeSentinelCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(LAAComponent);
    check(HealthComponent);
    check(SplineComponent);

    HealthComponent->OnCurrentHealthChanged.AddUObject(this, &ANTEyeSentinelCharacter::OnCurrentHealthChanged);

    HealthComponent->OnDeath.AddUObject(this, &ANTEyeSentinelCharacter::OnDeath);

    HealthComponent->OnActionThresholdReached.AddUObject(this, &ANTEyeSentinelCharacter::OnStrafeEnable);

    LAAComponent->SetViewTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    LAAComponent->SetEnable(false);
    LAAComponent->SetFollowSpeed(10.0f);
}

void ANTEyeSentinelCharacter::OnCurrentHealthChanged(float CurrentHealth)
{
    AAIController* AIController = GetController<AAIController>();

    check(AIController);

    HealthComponent->OnTakeDamageFromEnemy(AIController);

    LAAComponent->SetEnable(true);
}

void ANTEyeSentinelCharacter::OnDeath(bool GetAbility)
{
    if (DeathEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, GetActorLocation());
    }
    
    Destroy();
}

void ANTEyeSentinelCharacter::OnStrafeEnable()
{
    AAIController* AIController = GetController<AAIController>();

    check(AIController);
    AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsStrafeAvailable"), true);
}
