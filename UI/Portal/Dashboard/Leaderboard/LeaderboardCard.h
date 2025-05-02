// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardCard.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULeaderboardCard : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetPlayerInfo(const FString& Username, int32 Wins, int32 Place) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Wins;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Place;
};
