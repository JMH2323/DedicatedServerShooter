// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "HttpModule.h"
#include "DedicatedServers/Data/API/APIData.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/UI/HTTP/HTTPRequestTypes.h"
#include "Interfaces/IHttpResponse.h"
#include "DedicatedServers/GameplayTags/DedicatedServersTags.h"

void UPortalManager::JoinGameSession()
{

	BroadcastJoinGameSessionMessage.Broadcast(TEXT("Searching For Game..."), false);

	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::FindOrCreateGameSession_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();
	
}

void UPortalManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Find or Create Game Session Response Received");

	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
		DumpMetaData(JsonObject); // Could remove post-testing

		FDSGameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession);
		GameSession.Dump();

		BroadcastJoinGameSessionMessage.Broadcast(TEXT("Found Game Session."), false);
	}

	
}
