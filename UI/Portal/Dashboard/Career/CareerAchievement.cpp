﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CareerAchievement.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCareerAchievement::SetAchievementText(const FString& AchievementName, int32 AchievementValue) const
{
	TextBlock_AchievementName->SetText(FText::FromString(AchievementName));
	TextBlock_AchievementValue->SetText(FText::AsNumber(AchievementValue));
}

void UCareerAchievement::SetAchievementIcon(UTexture2D* Icon) const
{
	Image_AchievementIcon->SetBrushFromTexture(Icon);
}
