// NOVAE TERRAE. All Rights Reserved.


#include "Core/AI/NTBaseAIController.h"

void ANTBaseAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const 
{
    DetermineAISightPerceptionViewPoint(OutLocation, OutRotation);
}
