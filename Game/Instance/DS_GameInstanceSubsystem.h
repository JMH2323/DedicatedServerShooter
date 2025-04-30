// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameLiftServerSDK.h"
#include "DS_GameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDS_GameInstanceSubsystem();
	void InitGameLift(const FServerParameters& ServerParams);

	// Gamelift should only be initialized once
	UPROPERTY(BlueprintReadOnly)
	bool bGameLiftInitialized;

private:
	void ParseCommandLinePort(int32& OutPort);

	FProcessParameters ProcessParameters;
	
};
