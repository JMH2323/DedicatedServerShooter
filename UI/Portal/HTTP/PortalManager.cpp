// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

void UPortalManager::JoinGameSession()
{

	BroadcastJoinGameSessionMessage.Broadcast(TEXT("Searching For Game..."));
	
}
