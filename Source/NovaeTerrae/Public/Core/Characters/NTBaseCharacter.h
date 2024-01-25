// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NTBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNTHealthComponent;
class UNTThirstComponent;
class UTextRenderComponent;
class ANTCompanionPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDeathSignature, bool, IsLazer);

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

    UFUNCTION(BlueprintCallable, Category = "Companion")
    void BitCompanion();

    UPROPERTY(BlueprintAssignable);
    FOnResetDeathSignature OnResetDeathSignature;

    UPROPERTY(BlueprintAssignable);
    FOnDDeathSignature OnDeathSignature;

protected:
    virtual void BeginPlay() override;

    ANTCompanionPawn* Companion;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTHealthComponent* HealthComponentPrivet;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTThirstComponent* ThirstComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* ThirstTextComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "1000.0", ClampMax = "3500.0"))
    float LandedDeathVelocity = 1500.0f;

#pragma region Enhanced Inputs
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* RunAction;
    
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* DashAction;
#pragma endregion

private:
    bool bWantsToRun = false;
    bool bIsMovingForward = false;

#pragma region Dash
    FTimerHandle DashReseter;
    float ResetDashDelay = 1.5f;
    bool CanDash = true;
#pragma endregion

    FTimerHandle DeathTimer;

    UPROPERTY(EditDefaultsOnly, Category = "CameraAngle", meta = (ClampMin = "-60", ClampMax = "-30"))
    double MinCameraAngle;

    UPROPERTY(EditDefaultsOnly, Category = "CameraAngle", meta = (ClampMin = "10", ClampMax = "40"))
    double MaxCameraAngle;

    void Movement(const FInputActionValue& InputValue);
    void OnStopMovement();

    void Look(const FInputActionValue& InputValue);

    void OnStartRunnig();
    void OnStopRunnig();

    void OnDash();
    void OnResetDash();

    void OnResetDeath();

    void OnCurrentHealthChanged(float CurrentHealth);
    void OnDeath(bool IsLazer);

    void OnCurrentThirstChanged(float CurrentThirst);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
