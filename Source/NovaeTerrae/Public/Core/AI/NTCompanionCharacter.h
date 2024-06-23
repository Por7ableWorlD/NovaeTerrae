// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NTCompanionCharacter.generated.h"

class USphereComponent;
class UC3_LAA_MainComponent;
class UStaticMeshComponent;
class UNTCompanionHealthComponent;
class UTextRenderComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnThirstRemoveStart, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSacrificeStart, float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFastReloadStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFastReloadEnd);

UCLASS()
class NOVAETERRAE_API ANTCompanionCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANTCompanionCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTCompanionHealthComponent* CompanionHealthComponent;
    
    FOnThirstRemoveStart OnThirstRemoveStart;

    FOnSacrificeStart OnSacrificeStart;

    UPROPERTY(BlueprintAssignable);
    FOnFastReloadStart OnFastReloadStart;

    UPROPERTY(BlueprintAssignable);
    FOnFastReloadEnd OnFastReloadEnd;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills")
    float ThirstRemoveCost = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills")
    float ThirstRemoveHealthRestoration = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills", meta = (ClampMin = "0.0", ClampMax = "120.0"))
    float SacrificeCooldown = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float SacrificeCost = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills")
    float HealthSacrifice = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills", meta = (ClampMin = "0.0", ClampMax = "120.0"))
    float FastReloadCooldown = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float FastReloadCost = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills", meta = (ClampMin = "0.0", ClampMax = "20.0"))
    float FastReloadDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CompanionSkills", meta = (ClampMin = "0.0", ClampMax = "120.0"))
    float ScanCooldown = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTables")
    FName DataTableRowName = "DefaultSettings";

private:
    bool CanSacrifice = true;
    bool CanScan = true;
    bool CanFastReload = true;

    FTimerHandle SacrificeReseter;
    FTimerHandle ScanReseter;
    FTimerHandle FastReloadReseter;
    FTimerHandle FastReloadTimerHandle;

    void OnThirstRemoveRequest();
    void OnSacrificeRequest();
    void OnFastReloadRequest();
    void OnScanRequest();

    void OnSacrificeReset();
    void OnFastReloadReset();
    void OnScanReset();
};
