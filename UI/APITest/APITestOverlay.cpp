// Fill out your copyright notice in the Description page of Project Settings.


#include "APITestOverlay.h"

#include "APITestManager.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "DedicatedServers/UI/API/ListFleets/FleetId.h"
#include "DedicatedServers/UI/API/ListFleets/ListFleetsBox.h"
#include "DedicatedServers/UI/HTTP/HTTPRequestTypes.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(APITestManagerClass);
	APITestManager = NewObject<UAPITestManager>(this, APITestManagerClass);

	check(ListFleetsBox);
	check(ListFleetsBox->Button_ListFleets);
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(this, &UAPITestOverlay::ListFleetsButtonClicked);
	
}

void UAPITestOverlay::ListFleetsButtonClicked()
{
	check(APITestManager);
	APITestManager->OnListFleetsResponseRecieved.AddDynamic(this, &UAPITestOverlay::OnListFleetsResponseRecieved);
	APITestManager->ListFleets();
	ListFleetsBox->Button_ListFleets->SetIsEnabled(false);
}

void UAPITestOverlay::OnListFleetsResponseRecieved(const FDSListFleetsResponse& ListFleetsResponse, bool bWasSuccessful)
{
	if (APITestManager->OnListFleetsResponseRecieved.IsAlreadyBound(this, &UAPITestOverlay::OnListFleetsResponseRecieved))
	{
		APITestManager->OnListFleetsResponseRecieved.RemoveDynamic(this, &UAPITestOverlay::OnListFleetsResponseRecieved);
	}
	
	ListFleetsBox->ScrollBox_ListFleets->ClearChildren();
	if (bWasSuccessful)
	{
		for (const FString& FleetId : ListFleetsResponse.FleetIds)
		{
			UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
			FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString(FleetId));
			ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
		}
	}
	else
	{
		UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
		FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString("Error, Something Went Wrong"));
		ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
	}
	ListFleetsBox->Button_ListFleets->SetIsEnabled(true);
}
