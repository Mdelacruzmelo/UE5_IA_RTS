// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IA : ModuleRules
{
	public IA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"IA/Public/"
		});
	}
}