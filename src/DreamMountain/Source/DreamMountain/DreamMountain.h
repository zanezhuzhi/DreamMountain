// DreamMountain.h - 代号梦山主模块
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDreamMountainModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
