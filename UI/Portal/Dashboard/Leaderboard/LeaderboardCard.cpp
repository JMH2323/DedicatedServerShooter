// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderboardCard.h"
#include "Components/TextBlock.h"

void ULeaderboardCard::SetPlayerInfo(const FString& Username, int32 Wins, int32 Place) const
{
	TextBlock_Username->SetText(FText::FromString(Username));
	TextBlock_Wins->SetText(FText::AsNumber(Wins));
	TextBlock_Place->SetText(FText::AsNumber(Place));
}