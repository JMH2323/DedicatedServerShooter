// Fill out your copyright notice in the Description page of Project Settings.


#include "APITestManager.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"
#include "DedicatedServers/Data/API/APIData.h"
#include "DedicatedServers/GameplayTags/DedicatedServersTags.h"
#include "DedicatedServers/UI/HTTP/HTTPRequestTypes.h"
#include "Interfaces/IHttpResponse.h"

void UAPITestManager::ListFleets()
{

	check(APIData);
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UAPITestManager::ListFleets_Response);

	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::ListFleets);

	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "ListFleets_Request");
}

void UAPITestManager::ListFleets_Response(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bWasSuccesful)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ListFleets_Response");

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnListFleetsResponseRecieved.Broadcast(FDSListFleetsResponse(), false);
			return;
		}
		DumpMetaData(JsonObject);
		
		FDSListFleetsResponse ListFleetsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &ListFleetsResponse);
		ListFleetsResponse.Dump();

		OnListFleetsResponseRecieved.Broadcast(ListFleetsResponse, true);
	}

	
}
