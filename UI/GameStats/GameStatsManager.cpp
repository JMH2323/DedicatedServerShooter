// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "../HTTP/HTTPRequestTypes.h"
#include "../../Data/API/APIData.h"
#include "../../GameplayTags/DedicatedServersTags.h"

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
