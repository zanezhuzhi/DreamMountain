// DreamMountain.Target.cs - 游戏运行时目标
using UnrealBuildTool;
using System.Collections.Generic;

public class DreamMountainTarget : TargetRules
{
	public DreamMountainTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("DreamMountain");
	}
}
