// DMPlayerHUD.cpp - 绘制体力条

#include "DMPlayerHUD.h"

#include "DMCharacter.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"

void ADMPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas || !PlayerOwner)
	{
		return;
	}

	const ADMCharacter* Character = Cast<ADMCharacter>(PlayerOwner->GetPawn());
	if (!Character)
	{
		return;
	}

	const float StaminaPercent = Character->GetStaminaPercent();
	const float CurrentStamina = Character->GetCurrentStamina();
	const float MaxStamina = Character->GetMaxStamina();

	const float BarWidth = 320.0f;
	const float BarHeight = 22.0f;
	const float Padding = 2.0f;
	const float StartX = 40.0f;
	const float StartY = Canvas->ClipY - 70.0f;

	DrawRect(FLinearColor(0.05f, 0.05f, 0.05f, 0.75f), StartX, StartY, BarWidth, BarHeight);
	DrawRect(
		FLinearColor(0.10f, 0.80f, 0.35f, 0.95f),
		StartX + Padding,
		StartY + Padding,
		(BarWidth - Padding * 2.0f) * StaminaPercent,
		BarHeight - Padding * 2.0f
	);

	const FString Label = FString::Printf(TEXT("Stamina %.0f / %.0f"), CurrentStamina, MaxStamina);
	DrawText(Label, FLinearColor::White, StartX, StartY - 20.0f, nullptr, 1.0f, false);
}
