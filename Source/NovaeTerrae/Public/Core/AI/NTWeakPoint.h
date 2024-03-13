// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NTWeakPoint.generated.h"

class UNTEnemyHealthComponent;

UCLASS()
class NOVAETERRAE_API ANTWeakPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ANTWeakPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTEnemyHealthComponent* HealthComponent;

protected:
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "WeakPoint")
    void SetParameters(ACharacter* WeilderRef, float DeathAdditionalDamage);

private:
    void TransferDamage(float Damage);

	float AdditionalDamage = 10.0f;
    ACharacter* OwnerRef;
};
