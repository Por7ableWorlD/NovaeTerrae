// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NTThirstComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentThirstChangedSignature, float, NewThirst);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NOVAETERRAE_API UNTThirstComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNTThirstComponent();

    UPROPERTY(BlueprintAssignable);
    FOnCurrentThirstChangedSignature OnCurrentThirstChanged;

    UFUNCTION(BlueprintCallable, Category = "Thirst")
    float GetCurrentThirst() const { return CurrentThirst; }

    UFUNCTION(BlueprintCallable, Category = "Thirst")
    void SetThirst(float NewThirst);

    float GetThirstDuration() const { return ThirstDuration; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thirst", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    float MaxThirst = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thirst", meta = (ClampMin = "1.0", ClampMax = "60.0"))
    float ThirstDuration = 10.0f;

    virtual void BeginPlay() override;


private:
    float CurrentThirst = 2.0f;
};
