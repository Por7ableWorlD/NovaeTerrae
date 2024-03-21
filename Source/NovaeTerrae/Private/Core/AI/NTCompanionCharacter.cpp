// NOVAE TERRAE. All Rights Reserved.

#include "Core/AI/NTCompanionCharacter.h"
#include <Components/SphereComponent.h>
#include "Core/Characters/NTBaseCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <AIController.h>
#include "Core/Components/NTCompanionHealthComponent.h"
#include "Components/TextRenderComponent.h"

ANTCompanionCharacter::ANTCompanionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    CompanionHealthComponent = CreateDefaultSubobject<UNTCompanionHealthComponent>("CompanionHealthComponent");
}

void ANTCompanionCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CompanionHealthComponent);
    check(HealthTextComponent);

    OnCurrentHealthChanged(CompanionHealthComponent->GetCurrentHealth());
    CompanionHealthComponent->OnCurrentHealthChanged.AddUObject(this, &ANTCompanionCharacter::OnCurrentHealthChanged);
    CompanionHealthComponent->OnRegenerationFinished.AddUObject(this, &ANTCompanionCharacter::EnableActions);
    CompanionHealthComponent->OnDeath.AddDynamic(this, &ANTCompanionCharacter::DisableActions);

    ANTBaseCharacter* Player = Cast<ANTBaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!Player)
    {
        return;
    }
  
    Player->OnThirstRemoveSignature.AddUObject(this, &ANTCompanionCharacter::OnThirstRemoveRequest);
    Player->OnSacrificeRequestSignature.AddUObject(this, &ANTCompanionCharacter::OnSacrificeRequest);
    Player->OnFastReloadRequestSignature.AddUObject(this, &ANTCompanionCharacter::OnFastReloadRequest);
    Player->OnScanRequestSignature.AddUObject(this, &ANTCompanionCharacter::OnScanRequest);

    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    AIController->GetBlackboardComponent()->SetValueAsObject(FName("Player"), Player);
}

void ANTCompanionCharacter::OnCurrentHealthChanged(float CurrentHealth)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
}

void ANTCompanionCharacter::OnThirstRemoveRequest() 
{
    float SacrificedHealth = FMath::Clamp(ThirstRemoveHealthCost, 0.0f, CompanionHealthComponent->GetCurrentHealth());
    UGameplayStatics::ApplyDamage(this, SacrificedHealth, Controller, this, nullptr);
}

void ANTCompanionCharacter::OnSacrificeRequest()
{
    if (!CompanionHealthComponent->CheckHealthThreshold(SacrificeThreshold))
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("SacrificeThresholdReached"));
        return;
    }

    if (!CanSacrifice)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("SacrificeOnCooldown"));
        return;
    }
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("SacrificeSuccessful"));
    float SacrificedHealth = FMath::Clamp(HealthSacrifice, 0.0f, CompanionHealthComponent->GetCurrentHealth());

    OnSacrificeStart.Broadcast(SacrificedHealth);
    UGameplayStatics::ApplyDamage(this, SacrificedHealth, Controller, this, nullptr);

    if (SacrificeCooldown == 0)
    {
        return;
    }
    
    GetWorld()->GetTimerManager().SetTimer(SacrificeReseter, this, &ANTCompanionCharacter::OnSacrificeReset, SacrificeCooldown, false);
    CanSacrifice = false;
}

void ANTCompanionCharacter::OnFastReloadRequest() 
{
    if (!CompanionHealthComponent->CheckHealthThreshold(FastReloadCooldown))
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("FastReloadThresholdReached"));
        return;
    }

    if (!CanFastReload)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("FastReloadOnCooldown"));
        return;
    }
    
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("FastReloadSuccessful"));
    OnFastReloadStart.Broadcast();

    GetWorld()->GetTimerManager().SetTimer(
        FastReloadTimerHandle,
        [&]
        {
            OnFastReloadEnd.Broadcast();
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("FastReloadEnded"));
        },
        FastReloadDuration, false);

    if (FastReloadCooldown == 0)
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(FastReloadReseter, this, &ANTCompanionCharacter::OnFastReloadReset, FastReloadCooldown, false);
    CanFastReload = false;
}

void ANTCompanionCharacter::OnScanRequest()
{
    if (!CompanionHealthComponent->CheckHealthThreshold(ScanThreshold))
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("ScanThresholdReached"));
        return;
    }

    if (!CanScan)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("ScanOnCooldown"));
        return;
    }

    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("ScanSuccessful"));
    AIController->GetBlackboardComponent()->SetValueAsBool(FName("Scanning"), true);

    if (ScanCooldown == 0)
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(ScanReseter, this, &ANTCompanionCharacter::OnScanReset, ScanCooldown, false);
    CanScan = false;
}

void ANTCompanionCharacter::OnSacrificeReset() 
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("SacrificeCooldownReset"));
    CanSacrifice = true;
    GetWorld()->GetTimerManager().ClearTimer(SacrificeReseter);
}

void ANTCompanionCharacter::OnFastReloadReset()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("FastReloadCooldownReset"));
    CanFastReload = true;
    GetWorld()->GetTimerManager().ClearTimer(FastReloadReseter);
}

void ANTCompanionCharacter::OnScanReset()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ScanCooldownReset"));
    CanScan = true;
    GetWorld()->GetTimerManager().ClearTimer(ScanReseter);
}

void ANTCompanionCharacter::DisableActions(bool GetAbility) 
{
    AAIController* AIController = GetController<AAIController>();


    if (!AIController)
    {
        return;
    }

    AIController->GetBlackboardComponent()->SetValueAsBool(FName("Regenerating"), true);
}

void ANTCompanionCharacter::EnableActions() const 
{
    AAIController* AIController = GetController<AAIController>();

    if (!AIController)
    {
        return;
    }

    AIController->GetBlackboardComponent()->SetValueAsBool(FName("Regenerating"), false);
}
