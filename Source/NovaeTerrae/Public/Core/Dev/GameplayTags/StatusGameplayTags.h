// NOVAE TERRAE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct NOVAETERRAE_API FStatusGameplayTags : public FGameplayTagNativeAdder
{
    FGameplayTag Invulnerability;

    FORCEINLINE static const FStatusGameplayTags& Get() { return StatusTags; }

protected:
    virtual void AddTags() override
    {
        UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

        Invulnerability = Manager.AddNativeGameplayTag(TEXT("Status.Invulnerability"));
    }

private:
    static FStatusGameplayTags StatusTags;
};
