// DMGameModeBase.h - 代号梦山游戏模式
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DMGameModeBase.generated.h"

/**
 * 梦山默认游戏模式，使用 ADMCharacter 作为玩家控制的角色。
 */
UCLASS()
class DREAMMOUNTAIN_API ADMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADMGameModeBase();
};
