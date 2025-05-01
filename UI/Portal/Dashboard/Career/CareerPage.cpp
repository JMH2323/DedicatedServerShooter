// Fill out your copyright notice in the Description page of Project Settings.


#include "CareerPage.h"
#include "Components/TextBlock.h"


void UCareerPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Username->SetText(FText::GetEmpty());
	TextBlock_Wins->SetText(FText::AsNumber(0));
	TextBlock_Losses->SetText(FText::AsNumber(0));
	
}

void UCareerPage::OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse)
{
	
}

void UCareerPage::SetStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
}

