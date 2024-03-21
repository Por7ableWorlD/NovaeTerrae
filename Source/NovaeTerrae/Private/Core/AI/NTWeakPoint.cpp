// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTWeakPoint.h"
#include "Core/Components/NTEnemyHealthComponent.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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
    HealthComponent->OnDeath.AddDynamic(this, &ANTWeakPoint::OnDeath);
    HealthComponent->EnableActionThreshold = false;

	if (!IsChildActor())
	{
        return;
	}

	OwnerRef = GetParentActor();
}

void ANTWeakPoint::SetParameters(float WeakPointHealth, float DamageResistance, float DeathAdditionalDamage)
{
    HealthComponent->SetDefaultMaxHealth(WeakPointHealth);
    AdditionalDamage = DeathAdditionalDamage;
    HealthComponent->SetDamageResistancePercentage(DamageResistance);
}

void ANTWeakPoint::OnDeath(bool GetAbility) 
{
    if (OwnerRef)
    {
        UGameplayStatics::ApplyDamage(OwnerRef, AdditionalDamage, nullptr, this, TSubclassOf<UNTDevBaseDamageType>());
    }

	if (DeathEffect)
    {
        FTransform Transform = GetActorTransform();
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), DeathEffect, Transform.GetLocation(), Transform.Rotator(), Transform.GetScale3D());
    }

    Destroy();
    
}

void ANTWeakPoint::TransferDamage(float Damage) 
{
	if (!OwnerRef)
	{
        return;
	}

	UGameplayStatics::ApplyDamage(OwnerRef, Damage, nullptr, this, TSubclassOf<UNTDevBaseDamageType>());
}

