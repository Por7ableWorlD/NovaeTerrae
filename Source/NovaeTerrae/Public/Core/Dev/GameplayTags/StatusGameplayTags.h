// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct NOVAETERRAE_API FStatusGameplayTags : public FGameplayTagNativeAdder
{
    FGameplayTag Invulnerability;

    FGameplayTag Stun;

    FGameplayTag Thirst;

    FORCEINLINE static const FStatusGameplayTags& Get() { return StatusTags; }

protected:
    virtual void AddTags() override
    {
        UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

        Invulnerability = Manager.AddNativeGameplayTag(TEXT("Status.Invulnerability"));
        Stun = Manager.AddNativeGameplayTag(TEXT("Status.Stun"));
        Thirst = Manager.AddNativeGameplayTag(TEXT("Status.Thirst"));
    }

private:
    static FStatusGameplayTags StatusTags;
};
