// NOVAE TERRAE. All Rights Reserved.

#include "Core/AI/NTCompanionCharacter.h"
#include <C3_LAA_MainComponent.h>
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Core/Components/NTCompanionHealthComponent.h"
#include "Components/TextRenderComponent.h"

ANTCompanionCharacter::ANTCompanionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    LAAComponent = CreateDefaultSubobject<UC3_LAA_MainComponent>("LAAComponent");

    CompanionHealthComponent = CreateDefaultSubobject<UNTCompanionHealthComponent>("CompanionHealthComponent");
}

void ANTCompanionCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(LAAComponent);
    check(CompanionHealthComponent);
    check(HealthTextComponent);

    OnCurrentHealthChanged(CompanionHealthComponent->GetCurrentHealth());
    CompanionHealthComponent->OnCurrentHealthChanged.AddUObject(this, &ANTCompanionCharacter::OnCurrentHealthChanged);

    LAAComponent->SetViewTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    LAAComponent->SetDestinationOffset(FVector(0.0f, 0.0f, 50.0f));
}

void ANTCompanionCharacter::OnCurrentHealthChanged(float CurrentHealth)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
}
