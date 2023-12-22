// Copyright 2023 Cyrus 365. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C3_LAA_MainComponent.generated.h"

UCLASS(ClassGroup = (Custom), DisplayName = "Look At Component", meta = (BlueprintSpawnableComponent))
class C3_LAA_API UC3_LAA_MainComponent : public UActorComponent
{
    GENERATED_BODY()

#pragma region Protected Fields

protected:
    /** Is this "look at" function enabled? */
    UPROPERTY(EditAnywhere, Category = "LAA", DisplayName = "Enable")
    bool Enable = true;

    /** The actor to look at. */
    UPROPERTY(EditAnywhere, Category = "LAA", DisplayName = "View Target Actor")
    AActor* ViewTargetActor;

    /** 3D offset of destination. */
    UPROPERTY(EditAnywhere, Category = "LAA", DisplayName = "Destination Offset")
    FVector DestinationOffset;

    /** The speed of interp looking to ViewTargetActor. 0 means always center the target. Larger than 0 means it takes time to follow and
     * giving a smooth interp. */
    UPROPERTY(EditAnywhere, Category = "LAA", DisplayName = "Follow Speed")
    double FollowSpeed = 0.0;

#pragma endregion

#pragma region Protected Methods

protected:
    virtual void BeginPlay() override;

    /** (INTERNAL) */
    void UpdateView();

#pragma endregion

#pragma region Public Methods

public:
    UC3_LAA_MainComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Return if this component enabled. */
    UFUNCTION(BlueprintPure, Category = "LAA", DisplayName = "Is Enable")
    bool IsEnable() const;

    /** Enable component. */
    UFUNCTION(BlueprintCallable, Category = "LAA", DisplayName = "Set Enable")
    void SetEnable(const bool InEnable);

    /** Get current viewing target actor. */
    UFUNCTION(BlueprintPure, Category = "LAA", DisplayName = "Get View Target Actor")
    AActor* GetViewTargetActor() const;

    /** Set new viewing target actor. */
    UFUNCTION(BlueprintCallable, Category = "LAA", DisplayName = "Set View Target Actor")
    void SetViewTargetActor(AActor* const InViewTargetActor);

    /** Get offset location. */
    UFUNCTION(BlueprintCallable, Category = "LAA", DisplayName = "Get Destination Offset")
    FVector GetDestinationOffset() const;

    /** Set offset location. */
    UFUNCTION(BlueprintCallable, Category = "LAA", DisplayName = "Set Destination Offset")
    void SetDestinationOffset(const FVector& InDestinationOffset);

    /** Get interpolation speed. */
    UFUNCTION(BlueprintCallable, Category = "LAA", DisplayName = "Get Follow Speed")
    double GetFollowSpeed() const;

    /** Set interpolation speed. */
    UFUNCTION(BlueprintCallable, Category = "LAA", DisplayName = "Set Follow Speed")
    void SetFollowSpeed(const double InFollowSpeed);

#pragma endregion
};
