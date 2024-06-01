// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTBaseEnemyCharacter.h"
#include "Core/Components/NTEnemyHealthComponent.h"

ANTBaseEnemyCharacter::ANTBaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UNTEnemyHealthComponent>("HealthComponent");
}

void ANTBaseEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
}

