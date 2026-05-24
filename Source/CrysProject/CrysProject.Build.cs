// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrysProject : ModuleRules
{
	public CrysProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"DeveloperSettings", 
			"EnhancedInput", 
			"GameplayAbilities",
			"UMG",
			"StateTreeModule",
			"ModelViewViewModel",
			"UINavigation",
			"CrimAbilitySystem",
			"InventorySystem",
			"CrimBlueprintStatics",
			"CrimTargetingSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTags",
			"GameplayTasks",
			"NavigationSystem",
			"Niagara",
			"AnimGraphRuntime",
			"NetCore",
			"JsonUtilities",
			"Json",
			"GameplayMessageRuntime",
			"SlateCore",
			"AIModule",
			"TargetingSystem",
			"CrimBlueprintStatics", 
			// "InteractionSystem",
			"CrimAbilitySystem", 
			"InventorySystem", 
			"CrimEnhancedInputSystem"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
