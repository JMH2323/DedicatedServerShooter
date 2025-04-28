// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class ULeaderboardPage;
class UCareerPage;
class UGamePage;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGamePage> GamePage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCareerPage> CareerPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULeaderboardPage> LeaderboardPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CareerButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LeaderButton;

protected:
	virtual void NativeConstruct() override;

private:
	
	UFUNCTION()
	void ShowGamePage();
	
	UFUNCTION()
	void ShowCareerPage();

	UFUNCTION()
	void ShowLeaderboardPage();

	
};
