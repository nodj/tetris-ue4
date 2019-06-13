// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tetris_Game : ModuleRules
{
	public Tetris_Game(ReadOnlyTargetRules Target) : base(Target)
	{
//         OptimizeCode = 
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay"
            }
        );
	}
}
