// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NTEyeSentinelCharacter.generated.h"

class UC3_LAA_MainComponent;
class UStaticMeshComponent;
class UNTHealthComponent;

UCLASS()
class NOVAETERRAE_API ANTEyeSentinelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANTEyeSentinelCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UC3_LAA_MainComponent* LAAComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTHealthComponent* HealthComponent;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behavior", meta = (ClampMin = "0.1", ClampMax = "100.0"))
    float StrafeThresholdPercentage = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
    float ResurectionTime = 10.0f;

protected:
	virtual void BeginPlay() override;

    void OnCurrentHealthChanged(float CurrentHealth);

    void OnDeath();

private:
    FTimerHandle ResurectionTimerHandle;
    float ThresholdValue;
    int ThresholdNumber = 1;
};
