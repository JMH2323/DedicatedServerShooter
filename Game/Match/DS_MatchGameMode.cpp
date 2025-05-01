// Fill out your copyright notice in the Description page of Project Settings.


#include "DS_MatchGameMode.h"

#include "DedicatedServers/Player/DSPlayerController.h"
#include "DedicatedServers/Player/Match/DS_MatchPlayerState.h"

ADS_MatchGameMode::ADS_MatchGameMode()
{
	bUseSeamlessTravel = true;
	MatchStatus = EMatchStatus::WaitingForPlayers;
	PreMatchTimer.Type = ECountdownTimerType::PreMatch;
	MatchTimer.Type = ECountdownTimerType::Match;
	PostMatchTimer.Type = ECountdownTimerType::PostMatch;
}


void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		SetClientInputEnabled(false);
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADS_MatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);	
}

void ADS_MatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADS_MatchGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);

	if (Type == ECountdownTimerType::PreMatch)
	{
		MatchStatus = EMatchStatus::Match;
		StartCountdownTimer(MatchTimer);
		SetClientInputEnabled(true);
	}
	if (Type == ECountdownTimerType::Match)
	{
		MatchStatus = EMatchStatus::PostMatch;
		StartCountdownTimer(PostMatchTimer);
		SetClientInputEnabled(false);
	}
	if (Type == ECountdownTimerType::PostMatch)
	{
		MatchStatus = EMatchStatus::SeamlessTravelling;
		SetClientInputEnabled(true);
		TrySeamlessTravel(LobbyMap);
	}
}

void ADS_MatchGameMode::SetClientInputEnabled(bool bEnabled)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* PlayerController = Cast<ADSPlayerController>(*Iterator);
		if (IsValid(PlayerController))
		{
			PlayerController->Client_SetInputEnabled_Implementation(bEnabled);
		}
	}
}

void ADS_MatchGameMode::OnMatchEnded()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{				
		if (ADSPlayerController* PlayerController = Cast<ADSPlayerController>(*Iterator); IsValid(PlayerController))
		{
			if (ADS_MatchPlayerState* MatchPlayerState = PlayerController->GetPlayerState<ADS_MatchPlayerState>(); IsValid(MatchPlayerState))
			{
				MatchPlayerState->OnMatchEnded(PlayerController->Username);
			}
		}
	}
}
