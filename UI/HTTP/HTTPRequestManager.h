// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTPRequestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAPIStatusMessage, const FString&, StatusMessage, bool, bShouldResetWidgets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBroadcastJoinGameSessionMessage, const FString&, StatusMessage, bool, bResetJoinGameButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAPIRequestSucceeded);

class UAPIData;
class FJsonObject;
class UDSLocalPlayerSubsystem;
/**
 * 
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()

public:
	UDSLocalPlayerSubsystem* GetDSLocalPlayerSubsystem() const;

	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> APIData;

	bool ContainsErrors(TSharedPtr<FJsonObject> JsonObject);
	void DumpMetaData(TSharedPtr<FJsonObject> JsonObject);
	FString SerializeJsonObject(const TMap<FString, FString>& Params);

	
	
};
