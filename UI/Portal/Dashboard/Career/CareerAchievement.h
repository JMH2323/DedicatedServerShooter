// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CareerAchievement.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UCareerAchievement : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetAchievementText(const FString& AchievementName, int32 AchievementValue) const;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AchievementName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AchievementValue;
	
};
