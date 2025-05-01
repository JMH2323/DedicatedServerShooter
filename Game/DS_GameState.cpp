// Fill out your copyright notice in the Description page of Project Settings.


#include "DS_GameState.h"

#include "Lobby/LobbyState.h"
#include "Net/UnrealNetwork.h"

ADS_GameState::ADS_GameState()
{

	bReplicates = true;
	
}

void ADS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADS_GameState, LobbyState);
	
}

void ADS_GameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CreateLobbyState();
		OnLobbyStateInitialized.Broadcast(LobbyState);
	}
}

void ADS_GameState::CreateLobbyState()
{

	if (UWorld* World = GetWorld(); IsValid(World))
	{
		FActorSpawnParameters SpawnParams;
		LobbyState = World->SpawnActor<ALobbyState>(
			ALobbyState::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (IsValid(LobbyState))
		{
			LobbyState->SetOwner(this);
		}
	}
	
}

void ADS_GameState::OnRep_LobbyState()
{
	OnLobbyStateInitialized.Broadcast(LobbyState);
}
