// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "NTBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNTHealthComponent;
class UNTThirstComponent;
class UTextRenderComponent;
class ANTCompanionCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetDeathSignature);
DECLARE_MULTICAST_DELEGATE(FOnThirstRemoveSignature);
DECLARE_MULTICAST_DELEGATE(FOnSacrificeRequestSignature);
DECLARE_MULTICAST_DELEGATE(FOnFastReloadRequestSignature);
DECLARE_MULTICAST_DELEGATE(FOnScanRequestSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDeathSignature, AActor*, DeathCauser);

UCLASS()
class NOVAETERRAE_API ANTBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANTBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsWalking() const;

    UPROPERTY(BlueprintAssignable);
    FOnResetDeathSignature OnResetPlayerDeathSignature;

    UPROPERTY(BlueprintAssignable);
    FOnDDeathSignature OnPlayerDeathSignature;

    FOnThirstRemoveSignature OnThirstRemoveSignature;

    FOnSacrificeRequestSignature OnSacrificeRequestSignature;

    FOnFastReloadRequestSignature OnFastReloadRequestSignature;

    FOnScanRequestSignature OnScanRequestSignature;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
    FGameplayTagContainer GameTags;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Companion")
    ANTCompanionCharacter* Companion;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNTThirstComponent* ThirstComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "1000.0", ClampMax = "3500.0"))
    float LandedDeathVelocity = 1500.0f;

#pragma region Enhanced Inputs
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputMappingContext* LocomotionInputMappingContext;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputMappingContext* DefaultInputMappingContext;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputMappingContext* ExplorationInputMappingContext;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* WalkAction;
    
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* DashAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* ThirstRemoveAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* SacrificeAction;

    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* FastReloadAction;
    
    UPROPERTY(EditAnywhere, Category = "EnhancedInput")
    class UInputAction* ScanAction;
#pragma endregion

private:
    bool bWantsToWalk = false;

#pragma region Dash
    FTimerHandle DashReseter;
    float ResetDashDelay = 1.5f;
    bool CanDash = true;
#pragma endregion

    FTimerHandle DeathTimer;

    UPROPERTY(EditDefaultsOnly, Category = "CameraAngle", meta = (ClampMin = "-170", ClampMax = "-1"))
    double MinCameraAngle;

    UPROPERTY(EditDefaultsOnly, Category = "CameraAngle", meta = (ClampMin = "1", ClampMax = "170"))
    double MaxCameraAngle;

    void Movement(const FInputActionValue& InputValue);

    void Look(const FInputActionValue& InputValue);

    void OnStartWalking();
    void OnStopWalking();

    void OnDash();
    void OnResetDash();

    void OnThirstRemove();
    void ThirstRemoval(float HealthToRestore);

    void OnSacrifice();
    void SacrificingHeal(float SacrificedHealth);

    void OnScan();

    void OnFastReload();

    void OnResetDeath();

    UFUNCTION()
    void OnDeath(AActor* DeathCauser);

    void PlayDeathAnimation(AActor* DeathCauser, bool ResetThirst);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
