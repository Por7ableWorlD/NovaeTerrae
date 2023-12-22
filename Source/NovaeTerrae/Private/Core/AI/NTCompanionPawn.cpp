// NOVAE TERRAE. All Rights Reserved.

#include "Core/AI/NTCompanionPawn.h"
#include <C3_LAA_MainComponent.h>
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Core/Components/NTCompanionHealthComponent.h"
#include "Components/TextRenderComponent.h"

ANTCompanionPawn::ANTCompanionPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
    SetRootComponent(SphereCollisionComponent);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(SphereCollisionComponent);

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    LAAComponent = CreateDefaultSubobject<UC3_LAA_MainComponent>("LAAComponent");

    CompanionHealthComponent = CreateDefaultSubobject<UNTCompanionHealthComponent>("CompanionHealthComponent");
}

void ANTCompanionPawn::BeginPlay()
{
    Super::BeginPlay();

    check(LAAComponent);
    check(CompanionHealthComponent);
    check(HealthTextComponent);

    OnCurrentHealthChanged(CompanionHealthComponent->GetCurrentHealth());
    CompanionHealthComponent->OnCurrentHealthChanged.AddUObject(this, &ANTCompanionPawn::OnCurrentHealthChanged);

    LAAComponent->SetViewTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ANTCompanionPawn::OnCurrentHealthChanged(float CurrentHealth)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
}
