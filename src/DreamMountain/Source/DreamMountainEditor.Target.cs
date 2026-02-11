// DreamMountainEditor.Target.cs - 编辑器目标
using UnrealBuildTool;
using System.Collections.Generic;

public class DreamMountainEditorTarget : TargetRules
{
	public DreamMountainEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("DreamMountain");
		// UE 5.7: 安装版引擎下避免与 UnrealEditor 的属性冲突（Unique 仅适用于源码构建）
		bOverrideBuildEnvironment = true;
	}
}
