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

    HealthComponent->OnDeath.AddDynamic(this, &ANTDistortedHunter::OnDeath);
	
}

void ANTDistortedHunter::OnDeath(AActor* DeathCauser)
{
    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    DetachFromControllerPendingDestroy();

    if (!DeathAnimMontage)
    {
        return;
    }

    IsDead = true;

    DeathAnimMontage->bEnableAutoBlendOut = true;
    PlayAnimMontage(DeathAnimMontage);
}


