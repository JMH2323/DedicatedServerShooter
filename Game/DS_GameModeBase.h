﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DedicatedServers/Types/DSTypes.h"
#include "GameLiftServerSDK.h"
#include "DS_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:

	void StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void UpdateCountdownTimer(const FCountdownTimerHandle& CountdownTimerHandle);
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type);
	void TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap);
	void RemovePlayerSession(AController* Exiting);

};
