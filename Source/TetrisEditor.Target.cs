// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TetrisEditorTarget : TargetRules
{
	public TetrisEditorTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;

		Type = TargetType.Editor;
		ExtraModuleNames.Add("Tetris");

		bUseUnityBuild = false;
	}
}
