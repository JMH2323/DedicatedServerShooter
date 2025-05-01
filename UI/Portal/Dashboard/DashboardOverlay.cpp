// Fill out your copyright notice in the Description page of Project Settings.


#include "DashboardOverlay.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Career/CareerPage.h"
#include "Widgets/GamePage.h"
#include "DedicatedServers/UI/GameStats/GameStatsManager.h"
#include "Widgets/LeaderboardPage.h"


void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	GameButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	CareerButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	LeaderButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	GameStatsManager->OnRetrieveMatchStatsResponseReceived.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStats);
	GameStatsManager->RetrieveMatchStatsStatusMessage.AddDynamic(CareerPage, &UCareerPage::SetStatusMessage);
}

void UDashboardOverlay::ShowGamePage()
{
	DisableButton(GameButton);
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	DisableButton(CareerButton);
	WidgetSwitcher->SetActiveWidget(CareerPage);
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	DisableButton(LeaderButton);
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
}

void UDashboardOverlay::DisableButton(UButton* Button) const
{
	// Could probably be refactored.
	GameButton->SetIsEnabled(true);
	CareerButton->SetIsEnabled(true);
	LeaderButton->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}
