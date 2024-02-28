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

UCLASS()
class NOVAETERRAE_API ANTCompanionCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANTCompanionCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UC3_LAA_MainComponent* LAAComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTCompanionHealthComponent* CompanionHealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

protected:
    virtual void BeginPlay() override;

    void OnCurrentHealthChanged(float CurrentHealth);
};
