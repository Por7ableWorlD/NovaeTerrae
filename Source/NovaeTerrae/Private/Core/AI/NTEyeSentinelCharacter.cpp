// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTEyeSentinelCharacter.h"
#include "Core/Components/NTEnemyHealthComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
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

    HealthComponent = CreateDefaultSubobject<UNTEnemyHealthComponent>("HealthComponent");

    SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void ANTEyeSentinelCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(HealthComponent);
    check(SplineComponent);

    HealthComponent->OnTakeDamageFromPlayer.AddUObject(this, &ANTEyeSentinelCharacter::EnableAgressiveMode);

    HealthComponent->OnDeath.AddDynamic(this, &ANTEyeSentinelCharacter::OnDeath);

    HealthComponent->OnActionThresholdReached.AddUObject(this, &ANTEyeSentinelCharacter::OnStrafeEnable);
}

void ANTEyeSentinelCharacter::EnableAgressiveMode(float Damage)
{
    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    HealthComponent->OnTakeDamageFromEnemy(AIController);

}

void ANTEyeSentinelCharacter::OnDeath(bool GetAbility)
{
    if (DeathEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, GetActorLocation());
    }
    
    Destroy();
}

void ANTEyeSentinelCharacter::OnStrafeEnable() const
{
    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsStrafeAvailable"), true);
}
