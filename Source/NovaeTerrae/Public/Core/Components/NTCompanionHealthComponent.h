// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Components/NTHealthComponent.h"
#include "NTCompanionHealthComponent.generated.h"

UCLASS()
class NOVAETERRAE_API UNTCompanionHealthComponent : public UNTHealthComponent
{
    GENERATED_BODY()

public:
    UNTCompanionHealthComponent();

    bool CheckHealthThreshold(float SkillThreshold);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float StartHealDelay = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0.0"))
    float AutoHealStartLimit = 0.0f;

private:
    FTimerHandle HealTimerHandle;

    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
        AActor* DamageCauser) override;

    void HealUpdate();
};
