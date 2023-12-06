// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NTBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class NOVAETERRAE_API ANTBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANTBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    // Enhanced Inputs
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputMappingContext* InputMappingCpntext;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* LookAroundAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* RunAction;

private:
    bool bWantsToRun = false;
    bool bIsMovingForward = false;

    UPROPERTY(EditDefaultsOnly, Category = "CameraAngle", meta = (ClampMin = "-60", ClampMax = "-30"))
    double MinCameraAngle;

    UPROPERTY(EditDefaultsOnly, Category = "CameraAngle", meta = (ClampMin = "10", ClampMax = "40"))
    double MaxCameraAngle;

    void Movement(const FInputActionValue& InputValue);
    void OnStopMovement();

    void LookAround(const FInputActionValue& InputValue);

    void OnStartRunnig();
    void OnStopRunnig();
};
