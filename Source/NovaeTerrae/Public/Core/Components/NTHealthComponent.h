// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NTHealthComponent.generated.h"

class FHealthUpdate;
class FActorDeath;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNTHealthComponent();

    UFUNCTION(BlueprintCallable)
    void RestoreHealth(float Healing);

    UFUNCTION(BlueprintCallable)
    void TakeDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable)
    void ChangeDefaultMaxHealth(float NewDefaultHealth);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
    float MaxHealth;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Health")
    FHealthUpdate OnHealthUpdate;

    UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Health")
    FActorDeath OnActorDeath;

protected:
    virtual void BeginPlay() override;
};
