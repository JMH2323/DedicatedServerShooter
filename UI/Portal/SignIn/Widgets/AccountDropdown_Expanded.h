// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AccountDropdown_Expanded.generated.h"


class UButton;
class UTextBlock;
class UPortalManager;
class UDSLocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAccountDropdown_Expanded : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignOut;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SignOutButtonText;

	// TODO: (For real this time) Add Other data like Username, Stats
	// TODO: Stats might take longer.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor HoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor UnhoveredTextColor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
	UFUNCTION()
	void SignOutButton_Hover();

	UFUNCTION()
	void SignOutButton_Unhover();

	UFUNCTION()
	void SignOutButton_OnClicked();

private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	void SetSignOutButtonStyleTransparent();
	
};
