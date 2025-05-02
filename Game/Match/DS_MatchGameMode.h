// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DedicatedServers/Game/DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"


class UGameStatsManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:
	
	ADS_MatchGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	UPROPERTY()
	EMatchStatus MatchStatus;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

protected:
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	virtual void BeginPlay() override;
	void SetClientInputEnabled(bool bEnabled);
	void EndMatchForPlayerStates();
	virtual void OnMatchEnded();
	void UpdateLeaderboard(const TArray<FString>& LeaderboardNames);

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

	UFUNCTION()
	void OnLeaderboardUpdated();
	
private:

	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
};

