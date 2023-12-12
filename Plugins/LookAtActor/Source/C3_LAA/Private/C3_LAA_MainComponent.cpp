// Copyright 2023 Cyrus 365. All Rights Reserved.


#include "C3_LAA_MainComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UC3_LAA_MainComponent::UC3_LAA_MainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UC3_LAA_MainComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UC3_LAA_MainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateView();
}

void UC3_LAA_MainComponent::UpdateView()
{
	// Look at Target
	if (Enable && ViewTargetActor)
	{
		// Pre Calculate
		const FVector tempDesLoc = ViewTargetActor->GetActorLocation() + DestinationOffset;
		const FRotator tempDesRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), tempDesLoc);
		FRotator tempFinalRot;

		// Need Interp? 
		if (FollowSpeed > 0.0)
		{
			tempFinalRot = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), tempDesRot, GetWorld()->GetDeltaSeconds(), FollowSpeed);
		}
		else
		{
			tempFinalRot = tempDesRot;
		}

		// Main Action
		GetOwner()->SetActorRotation(tempFinalRot);
	}
}

bool UC3_LAA_MainComponent::IsEnable() const
{
	return Enable;
}

void UC3_LAA_MainComponent::SetEnable(const bool InEnable)
{
	Enable = InEnable;
}

AActor* UC3_LAA_MainComponent::GetViewTargetActor() const
{
	return ViewTargetActor;
}

void UC3_LAA_MainComponent::SetViewTargetActor(AActor* const InViewTargetActor)
{
	this->ViewTargetActor = InViewTargetActor;
}

FVector UC3_LAA_MainComponent::GetDestinationOffset() const
{
	return DestinationOffset;
}

void UC3_LAA_MainComponent::SetDestinationOffset(const FVector& InDestinationOffset)
{
	this->DestinationOffset = InDestinationOffset;
}

double UC3_LAA_MainComponent::GetFollowSpeed() const
{
	return FollowSpeed;
}

void UC3_LAA_MainComponent::SetFollowSpeed(const double InFollowSpeed)
{
	this->FollowSpeed = InFollowSpeed;
}


