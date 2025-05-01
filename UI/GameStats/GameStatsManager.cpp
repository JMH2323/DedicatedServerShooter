// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "../HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/Player/DSLocalPlayerSubsystem.h"
#include "../../Data/API/APIData.h"
#include "../../GameplayTags/DedicatedServersTags.h"
#include "DedicatedServers/DedicatedServers.h"

void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput RecordMatchStatsInput)
{

	
	// For JsonObject...
	//	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	//	FJsonObjectConverter::UStructToJsonObject(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonObject.ToSharedRef());

	// For String...
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
	
}


void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                  bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats failed."))
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		ContainsErrors(JsonObject);
	}
}


void UGameStatsManager::RetrieveMatchStats()
{
	RetrieveMatchStatsStatusMessage.Broadcast(TEXT("Retrieving stats..."), false);
	
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	check(APIData);
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RetrieveMatchStats);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	TMap<FString, FString> Params = {
		{ TEXT("accessToken"), LocalPlayerSubsystem->GetAuthResult().AccessToken }
	};
	const FString Content = SerializeJsonObject(Params);
	
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UGameStatsManager::RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrieveMatchStatsResponseReceived.Broadcast(FDSRetrieveMatchStatsResponse());
		RetrieveMatchStatsStatusMessage.Broadcast(TEXT("Something went wrong"), false);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnRetrieveMatchStatsResponseReceived.Broadcast(FDSRetrieveMatchStatsResponse());
			return;
		}

		FDSRetrieveMatchStatsResponse RetrieveMatchStatsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveMatchStatsResponse);
		RetrieveMatchStatsResponse.Dump();

		OnRetrieveMatchStatsResponseReceived.Broadcast(RetrieveMatchStatsResponse);
	}
}