// DMPlayerHUD.h - 代号梦山玩家 HUD（体力条）
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DMPlayerHUD.generated.h"

UCLASS()
class DREAMMOUNTAIN_API ADMPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
