// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTRocketSentinelCharacter.h"
#include "Core/Components/NTEnemyHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <C3_LAA_MainComponent.h>
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <AIController.h>

DEFINE_LOG_CATEGORY_STATIC(LogRocketSentinel, All, All)

ANTRocketSentinelCharacter::ANTRocketSentinelCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    LAAComponent = CreateDefaultSubobject<UC3_LAA_MainComponent>("LAAComponent");

    HealthComponent = CreateDefaultSubobject<UNTEnemyHealthComponent>("HealthComponent");
}

void ANTRocketSentinelCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(LAAComponent);
    check(HealthComponent);

    HealthComponent->OnCurrentHealthChanged.AddUObject(this, &ANTRocketSentinelCharacter::OnCurrentHealthChanged);

    HealthComponent->OnDeath.AddUObject(this, &ANTRocketSentinelCharacter::OnDeath);

    HealthComponent->OnActionThresholdReached.AddUObject(this, &ANTRocketSentinelCharacter::OnShieldEnable);

    LAAComponent->SetViewTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    LAAComponent->SetEnable(false);
    LAAComponent->SetFollowSpeed(10.0f);
	
}

void ANTRocketSentinelCharacter::OnCurrentHealthChanged(float CurrentHealth)
{
    AAIController* AIController = GetController<AAIController>();

    check(AIController);

    HealthComponent->OnTakeDamageFromEnemy(AIController);

    LAAComponent->SetEnable(true);
}

void ANTRocketSentinelCharacter::OnDeath(bool GetAbility)
{
    if (DeathEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, GetActorLocation());
    }

    Destroy();
}

void ANTRocketSentinelCharacter::OnShieldEnable()
{
    AAIController* AIController = GetController<AAIController>();

    check(AIController);
    AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsShieldAvailable"), true);
}