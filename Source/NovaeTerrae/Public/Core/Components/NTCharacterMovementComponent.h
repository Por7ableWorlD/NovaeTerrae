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
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float WalkModifier = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float AimModifier = 0.5f;


    virtual float GetMaxSpeed() const override;
};
