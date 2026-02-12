// DMGameModeBase.cpp - 代号梦山游戏模式
#include "DMGameModeBase.h"
#include "DMCharacter.h"
#include "DMPlayerHUD.h"

ADMGameModeBase::ADMGameModeBase()
{
	DefaultPawnClass = ADMCharacter::StaticClass();
	HUDClass = ADMPlayerHUD::StaticClass();
}
