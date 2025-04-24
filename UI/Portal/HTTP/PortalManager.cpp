// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "HttpModule.h"
#include "DedicatedServers/Data/API/APIData.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/UI/HTTP/HTTPRequestTypes.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "DedicatedServers/GameplayTags/DedicatedServersTags.h"



void UPortalManager::SignIn(const FString& Username, const FString& Password)
{
	
}

void UPortalManager::SignUp(const FString& Username, const FString& Password, const FString& Email)
{
	
}

void UPortalManager::Confirm(const FString& ConfirmationCode)
{
	
}

void UPortalManager::QuitGame()
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}