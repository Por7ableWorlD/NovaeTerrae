// NOVAE TERRAE. All Rights Reserved.

#include "Core/NTGameModeBase.h"
#include "Core/NTBasePlayerController.h"
#include "Core/Characters/NTBaseCharacter.h"

ANTGameModeBase::ANTGameModeBase()
{
    DefaultPawnClass = ANTBaseCharacter::StaticClass();
    PlayerControllerClass = ANTBasePlayerController::StaticClass();
}
