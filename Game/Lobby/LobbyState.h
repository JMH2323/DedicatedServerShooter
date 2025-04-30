// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "LobbyPlayerInfo.h"
#include "LobbyState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChanged, const FLobbyPlayerInfo&, PlayerInfo);

USTRUCT()
struct FLobbyPlayerInfoDelta
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLobbyPlayerInfo> AddedPlayers{};

	UPROPERTY()
	TArray<FLobbyPlayerInfo> RemovedPlayers{};
};

UCLASS()
class DEDICATEDSERVERS_API ALobbyState : public AInfo
{
	GENERATED_BODY()

public:
	ALobbyState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoAdded;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoRemoved;

	void AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo);
	void RemovePlayerInfo(const FString& Username);
	
protected:

	UFUNCTION()
	void OnRep_LobbyPlayerInfo();
	
private:
	
	UPROPERTY(ReplicatedUsing=OnRep_LobbyPlayerInfo)
	FLobbyPlayerInfoArray PlayerInfoArray;

	UPROPERTY()
	FLobbyPlayerInfoArray LastPlayerInfoArray;

	FLobbyPlayerInfoDelta ComputePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& OldArray, const TArray<FLobbyPlayerInfo>& NewArray);
	
};
