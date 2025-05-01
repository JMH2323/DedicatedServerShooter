// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CareerPage.generated.h"

struct FDSRetrieveMatchStatsResponse;
class UScrollBox;
class UCareerAchievement;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UCareerPage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Wins;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Losses;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Achievements;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCareerAchievement> CareerAchievementClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;

	UFUNCTION()
	void OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse);

	UFUNCTION()
	void SetStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets);
	
protected:
	virtual void NativeConstruct() override;
	
};
