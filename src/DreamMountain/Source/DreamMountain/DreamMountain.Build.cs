// DreamMountain.Build.cs - 代号梦山模块编译配置
using UnrealBuildTool;

public class DreamMountain : ModuleRules
{
	public DreamMountain(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;  // UE 5.7 不再支持 Cpp17

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});
	}
}
