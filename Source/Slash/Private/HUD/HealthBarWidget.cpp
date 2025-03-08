// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidget.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
