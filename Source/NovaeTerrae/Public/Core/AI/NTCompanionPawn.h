// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NTCompanionPawn.generated.h"

class USphereComponent;
class UC3_LAA_MainComponent;
class UStaticMeshComponent;
class UNTCompanionHealthComponent;
class UTextRenderComponent;

UCLASS()
class NOVAETERRAE_API ANTCompanionPawn : public APawn
{
    GENERATED_BODY()

public:
    ANTCompanionPawn();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereCollisionComponent;

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
