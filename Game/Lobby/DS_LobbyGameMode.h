// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_LobbyGameMode.generated.h"


class UDS_GameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:
	
	ADS_LobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	void CheckAndCancelCountdown();
	virtual void Logout(AController* Exiting) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	// After PreLogin. This happens after the player controller is assigned to a new player
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;

	// Alternative to PostLogin, as Post Login is not called on seamless travel
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	UPROPERTY()
	ELobbyStatus LobbyStatus;

	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MapToTravelTo;

private:

	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer;

	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
	void AddPlayerInfoToLobbyState(AController* Player) const;
	void RemovePlayerInfoFromLobbyState(AController* Player) const;
	
};

