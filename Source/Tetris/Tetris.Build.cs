// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tetris : ModuleRules
{
	public Tetris(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

//         OptimizeCode = CodeOptimization.Never;
//         bFasterWithoutUnity = true;
//         PCHUsage = PCHUsageMode.NoPCHs;

        PrivateIncludePaths.Add("Tetris/core");

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
				"CoreUObject",
				"Engine",
				"InputCore",
            }
        );

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
