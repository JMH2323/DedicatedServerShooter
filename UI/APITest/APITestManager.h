// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DedicatedServers/UI/HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "APITestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFleetsResponseRecieved, const FDSListFleetsResponse&, ListFleetsResponse, bool, bWasSuccessful);
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void ListFleets();

	UPROPERTY()
	FOnListFleetsResponseRecieved OnListFleetsResponseRecieved;
	
	void ListFleets_Response(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bWasSuccesful);
	
	
};
