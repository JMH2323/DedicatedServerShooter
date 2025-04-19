// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DedicatedServers/GameplayTags/DedicatedServersTags.h"

#include "APIData.generated.h"


/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPIData : public UDataAsset
{
	GENERATED_BODY()


public:

	FString GetAPIEndpoint(const FGameplayTag& APIEndpoint);

	
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	FString Stage;

	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> Resources;
};
