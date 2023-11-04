// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NTBaseCharacter.generated.h"

class UCameraComponent;

UCLASS()
class NOVAETERRAE_API ANTBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANTBaseCharacter();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;
};
