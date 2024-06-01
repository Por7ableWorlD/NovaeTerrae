// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "NTBaseEnemyCharacter.generated.h"

class UNTEnemyHealthComponent;

UCLASS()
class NOVAETERRAE_API ANTBaseEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANTBaseEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTEnemyHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayTags")
    FGameplayTagContainer GameTags;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTables")
    FName DataTableRowName = "DefaultSettings";

    virtual void BeginPlay() override;

};
