// Last modified: 2021/04/21 21:00:00

#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthPercent(float Percent)
{
    if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetStaminaPercent(float Percent)
{  
    if (StaminaProgressBar)
    {
        StaminaProgressBar->SetPercent(Percent);
    }
}
void USlashOverlay::SetGold(int32 Gold)
{
    if (GoldText)
    {
        const FString SString = FString::Printf(TEXT("%d"), Gold);
        const FText TText = FText::FromString(SString);
        GoldText->SetText(TText);
    }
    
}

void USlashOverlay::SetSouls(int32 Souls)
{
    if (SoulsText)
    {
        const FString SString = FString::Printf(TEXT("%d"), Souls);
        const FText TText = FText::FromString(SString);
        SoulsText->SetText(TText);
    }
}
