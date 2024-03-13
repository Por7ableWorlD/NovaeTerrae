// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTWeakPoint.h"
#include "Core/Components/NTEnemyHealthComponent.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include "Core/Dev/Damage/NTDevBaseDamageType.h"


ANTWeakPoint::ANTWeakPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UNTEnemyHealthComponent>("HealthComponent");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());
}

void ANTWeakPoint::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnTakeDamageFromPlayer.AddUObject(this, &ANTWeakPoint::TransferDamage);
    HealthComponent->EnableActionThreshold = false;
}

void ANTWeakPoint::SetParameters(ACharacter* WeilderRef, float DeathAdditionalDamage) 
{
    OwnerRef = WeilderRef;
    AdditionalDamage = DeathAdditionalDamage;
}

void ANTWeakPoint::TransferDamage(float Damage) 
{
	if (!OwnerRef)
	{
        return;
	}

    if (HealthComponent->IsDead())
    {
        Damage += AdditionalDamage;
    }

	UGameplayStatics::ApplyDamage(OwnerRef, Damage, nullptr, this, TSubclassOf<UNTDevBaseDamageType>());
}

