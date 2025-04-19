// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UListFleetsBox;
class UFleetId;
class UAPITestManager;
struct FDSListFleetsResponse;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPITestManager> APITestManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFleetId> FleetIdWidgetClass;

protected:
	virtual void NativeConstruct() override;

	
private:

	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListFleetsBox> ListFleetsBox;
	
	UPROPERTY()
	TObjectPtr<UAPITestManager> APITestManager;

	UFUNCTION()
	void ListFleetsButtonClicked();

	UFUNCTION()
	void OnListFleetsResponseRecieved(const FDSListFleetsResponse& ListFleetsResponse, bool bWasSuccessful);
	
};
