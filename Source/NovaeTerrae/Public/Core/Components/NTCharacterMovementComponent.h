// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NTCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class NOVAETERRAE_API UNTCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.1", ClampMax = "3.0"))
    float RunModifier = 2.0f;

    virtual float GetMaxSpeed() const override;
};
