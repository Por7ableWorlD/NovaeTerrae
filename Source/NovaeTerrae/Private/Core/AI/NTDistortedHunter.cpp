// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTDistortedHunter.h"
#include "Core/Components/NTEnemyHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <AIController.h>

ANTDistortedHunter::ANTDistortedHunter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    HealthComponent = CreateDefaultSubobject<UNTEnemyHealthComponent>("HealthComponent");

}

void ANTDistortedHunter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);

    HealthComponent->OnTakeDamageFromPlayer.AddUObject(this, &ANTDistortedHunter::EnableAgressiveMode);

    HealthComponent->OnDeath.AddDynamic(this, &ANTDistortedHunter::OnDeath);
	
}

void ANTDistortedHunter::EnableAgressiveMode(float Damage)
{
    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    HealthComponent->OnTakeDamageFromEnemy(AIController);
}

void ANTDistortedHunter::OnDeath(bool GetAbility)
{
    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    AIController->UnPossess();

    if (!DeathAnimMontage)
    {
        return;
    }

    IsDead = true;

    DeathAnimMontage->bEnableAutoBlendOut = true;
    PlayAnimMontage(DeathAnimMontage);
}


