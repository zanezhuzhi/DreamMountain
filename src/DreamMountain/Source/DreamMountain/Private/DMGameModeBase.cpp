// DMGameModeBase.cpp - 代号梦山游戏模式
#include "DMGameModeBase.h"
#include "DMCharacter.h"

ADMGameModeBase::ADMGameModeBase()
{
	DefaultPawnClass = ADMCharacter::StaticClass();
}
