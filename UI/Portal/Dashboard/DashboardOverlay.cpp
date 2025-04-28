// Fill out your copyright notice in the Description page of Project Settings.


#include "DashboardOverlay.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/CareerPage.h"
#include "Widgets/GamePage.h"
#include "Widgets/LeaderboardPage.h"


void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	GameButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	CareerButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	LeaderButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);
	
}

void UDashboardOverlay::ShowGamePage()
{
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	WidgetSwitcher->SetActiveWidget(CareerPage);
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
}
