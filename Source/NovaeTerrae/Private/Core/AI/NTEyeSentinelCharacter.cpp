// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTEyeSentinelCharacter.h"
#include "Core/Components/NTHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include <C3_LAA_MainComponent.h>
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SplineComponent.h"
#include <AIController.h>

DEFINE_LOG_CATEGORY_STATIC(LogEyeSentinel, All, All)

ANTEyeSentinelCharacter::ANTEyeSentinelCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    LAAComponent = CreateDefaultSubobject<UC3_LAA_MainComponent>("LAAComponent");

    HealthComponent = CreateDefaultSubobject<UNTHealthComponent>("HealthComponent");

    SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void ANTEyeSentinelCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(LAAComponent);
    check(HealthComponent);
    check(SplineComponent);

    HealthComponent->OnCurrentHealthChanged.AddUObject(this, &ANTEyeSentinelCharacter::OnCurrentHealthChanged);

    HealthComponent->OnDeath.AddUObject(this, &ANTEyeSentinelCharacter::OnDeath);

    LAAComponent->SetViewTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    LAAComponent->SetEnable(false);
    LAAComponent->SetDestinationOffset({0.0f, 0.0f, -50.0f});
    LAAComponent->SetFollowSpeed(3.0f);

    GetCharacterMovement()->GravityScale = 0.0f;

    ThresholdValue = HealthComponent->GetMaxHealth() * (1 - (StrafeThresholdPercentage * ThresholdNumber) / 100);
}

void ANTEyeSentinelCharacter::OnCurrentHealthChanged(float CurrentHealth)
{
    UE_LOG(LogEyeSentinel, Display, TEXT("[DEBUG] Current Health: %.0f"), CurrentHealth);
    UE_LOG(LogEyeSentinel, Display, TEXT("[DEBUG] Current Threshold: %.0f"), ThresholdValue);

    AAIController* AIController = GetController<AAIController>();

    check(AIController);

    AIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    LAAComponent->SetEnable(true);

    if (CurrentHealth > ThresholdValue)
    {
        return;
    }

    AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsStrafeAvailable"), true);


    float NewThreshold = HealthComponent->GetMaxHealth() * (1 - (StrafeThresholdPercentage * (ThresholdNumber + 1)) / 100);

    if (NewThreshold < 0)
    {
        return;
    }

    ThresholdValue = NewThreshold;
    ++ThresholdNumber;
    UE_LOG(LogEyeSentinel, Display, TEXT("[DEBUG] Old Threshold: %.0f"), ThresholdValue);
    UE_LOG(LogEyeSentinel, Display, TEXT("[DEBUG] New Threshold: %.0f"), NewThreshold);
}

void ANTEyeSentinelCharacter::OnDeath(bool GetAbility)
{
    Destroy();
}
