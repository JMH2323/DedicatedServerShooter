// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyState.h"
#include "Net/UnrealNetwork.h"


ALobbyState::ALobbyState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALobbyState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyState, PlayerInfoArray);
}

void ALobbyState::AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerInfoArray.AddPlayer(PlayerInfo);
}

void ALobbyState::RemovePlayerInfo(const FString& Username)
{
	PlayerInfoArray.RemovePlayer(Username);
}

void ALobbyState::OnRep_LobbyPlayerInfo()
{
	FLobbyPlayerInfoDelta Delta = ComputePlayerInfoDelta(LastPlayerInfoArray.Players, PlayerInfoArray.Players);
	for (const auto& PlayerInfo : Delta.AddedPlayers)
	{
		OnPlayerInfoAdded.Broadcast(PlayerInfo);
	}
	for (const auto& PlayerInfo : Delta.RemovedPlayers)
	{
		OnPlayerInfoRemoved.Broadcast(PlayerInfo);
	}

	LastPlayerInfoArray = PlayerInfoArray;
}

FLobbyPlayerInfoDelta ALobbyState::ComputePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& OldArray,
	const TArray<FLobbyPlayerInfo>& NewArray)
{
	FLobbyPlayerInfoDelta Delta;

	// Tmaps .Contains() Is faster than comparing Arrays
	TMap<FString, const FLobbyPlayerInfo*> OldMap;
	TMap<FString, const FLobbyPlayerInfo*> NewMap;

	// Populate maps 
	for (const auto& PlayerInfo : OldArray)
	{
		OldMap.Add(PlayerInfo.Username, &PlayerInfo);
	}
	for (const auto& PlayerInfo : NewArray)
	{
		NewMap.Add(PlayerInfo.Username, &PlayerInfo);
	}
	
	for (const auto& OldPlayerInfo : OldArray)
	{
		// Item doesn't exist in new array (Player removed)
		if (!NewMap.Contains(OldPlayerInfo.Username))
		{
			Delta.RemovedPlayers.Add(OldPlayerInfo);
		}
	}
	for (const auto& NewPlayerInfo : NewArray)
	{
		// Item doesn't exist in the old array (Player added)
		if (!OldMap.Contains(NewPlayerInfo.Username))
		{
			Delta.AddedPlayers.Add(NewPlayerInfo);
		}
	}

	return Delta;
}


