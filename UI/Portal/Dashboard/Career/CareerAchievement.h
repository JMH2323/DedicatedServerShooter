// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CareerAchievement.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UCareerAchievement : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetAchievementText(const FString& AchievementName, int32 AchievementValue) const;
	void SetAchievementIcon(UTexture2D* Icon) const;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AchievementName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AchievementValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_AchievementIcon;
	
};
