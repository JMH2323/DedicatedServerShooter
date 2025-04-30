// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DedicatedServers : ModuleRules
{
	public DedicatedServers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "HTTP", "NetCore", "GameplayTags", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PhysicsCore", "GameLiftServerSDK" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "Json", "JsonUtilities", "SlateCore", "UMG" });
	}
}
