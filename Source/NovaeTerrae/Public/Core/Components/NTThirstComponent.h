// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NTThirstComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentThirstChangedSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTThirstComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNTThirstComponent();

    FOnCurrentThirstChangedSignature OnCurrentThirstChanged;

    float GetCurrentThirst() const { return CurrentThirst; }

protected:
    virtual void BeginPlay() override;

    void SetThirst(float NewThirst);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thirst", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    float MaxThirst = 5.0f;

private:
    float CurrentThirst = 2.0f;
};
