// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NovaeTerrae : ModuleRules
{
	public NovaeTerrae(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "FMODStudio", "EnhancedInput", "C3_LAA", "LineOfSight" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] { "NovaeTerrae/Public/Core", "NovaeTerrae/Public/Core/Characters",
            "NovaeTerrae/Public/Core/Components", "NovaeTerrae/Public/Core/AI", "NovaeTerrae/Public/Core/Dev/Damage"});

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
