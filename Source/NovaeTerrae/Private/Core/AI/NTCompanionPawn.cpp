// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTCompanionPawn.h"
#include <C3_LAA_MainComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include "GameFramework/Character.h"

ANTCompanionPawn::ANTCompanionPawn()
{
	PrimaryActorTick.bCanEverTick = true;

    SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
    SphereCollisionComponent.Get()->SetSphereRadius(50.f);
    SphereCollisionComponent.Get()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly, true);

    LAAComponent = CreateDefaultSubobject<UC3_LAA_MainComponent>(TEXT("LAA Component"));

}

void ANTCompanionPawn::BeginPlay()
{
	Super::BeginPlay();

    if (!IsValid(LAAComponent.Get()))
    {
        return;
    }
	
    LAAComponent.Get()->SetViewTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

