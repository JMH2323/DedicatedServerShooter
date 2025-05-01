// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerBox.h"

#include "Components/ScrollBox.h"
#include "DedicatedServers/Game/DS_GameState.h"
#include "DedicatedServers/Game/Lobby/LobbyState.h"
#include "DedicatedServers/Game/Lobby/LobbyPlayerInfo.h"
#include "DedicatedServers/UI/Lobby/PlayerLabel.h"

void ULobbyPlayerBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADS_GameState* DSGameState = GetWorld()->GetGameState<ADS_GameState>();
	if (!IsValid(DSGameState)) return;

	if (IsValid(DSGameState->LobbyState))
	{
		
	} else
	{
		DSGameState->OnLobbyStateInitialized.AddDynamic(this, &ULobbyPlayerBox::OnLobbyStateInitialized);
	}
	
}



void ULobbyPlayerBox::OnLobbyStateInitialized(ALobbyState* LobbyState)
{
	// When initialized, bind new players joining and leaving to our functions here
	if (!IsValid(LobbyState)) return;
	LobbyState->OnPlayerInfoAdded.AddDynamic(this, &ULobbyPlayerBox::CreateAndAddPlayerLabel);
	LobbyState->OnPlayerInfoRemoved.AddDynamic(this, &ULobbyPlayerBox::OnPlayerRemoved);

	// Then update the initial player info by each player.
	UpdatePlayerInfo(LobbyState);
}

void ULobbyPlayerBox::UpdatePlayerInfo(ALobbyState* LobbyState)
{
	ScrollBox_PlayerInfo->ClearChildren();
	for (const FLobbyPlayerInfo& PlayerInfo : LobbyState->GetPlayers())
	{
		CreateAndAddPlayerLabel(PlayerInfo);
	}
}

void ULobbyPlayerBox::CreateAndAddPlayerLabel(const FLobbyPlayerInfo& PlayerInfo)
{
	// Prevents duplicates
	if (FindPlayerLabel(PlayerInfo.Username)) return;
	
	UPlayerLabel* PlayerLabel = CreateWidget<UPlayerLabel>(this, PlayerLabelClass);
	if (!IsValid(PlayerLabel)) return;
	PlayerLabel->SetUsername(PlayerInfo.Username);
	ScrollBox_PlayerInfo->AddChild(PlayerLabel);
}

void ULobbyPlayerBox::OnPlayerRemoved(const FLobbyPlayerInfo& PlayerInfo)
{
	if (UPlayerLabel* PlayerLabel = FindPlayerLabel(PlayerInfo.Username))
	{
		ScrollBox_PlayerInfo->RemoveChild(PlayerLabel);
	}
}

UPlayerLabel* ULobbyPlayerBox::FindPlayerLabel(const FString& Username)
{
	for (UWidget* Child : ScrollBox_PlayerInfo->GetAllChildren())
	{
		UPlayerLabel* Playerlabel = Cast<UPlayerLabel>(Child);
		if (Playerlabel->GetUsername() == Username)
		{
			return Playerlabel;
		}
	}
	return nullptr;
}



