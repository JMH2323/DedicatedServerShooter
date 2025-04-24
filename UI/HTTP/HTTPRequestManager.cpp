// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPRequestManager.h"

#include "HTTPRequestTypes.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/Player/DSLocalPlayerSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"

UDSLocalPlayerSubsystem* UHTTPRequestManager::GetDSLocalPlayerSubsystem() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(LocalPlayerController->Player);
		if (IsValid(LocalPlayer))
		{
			return LocalPlayer->GetSubsystem<UDSLocalPlayerSubsystem>();
		}
	}
	return nullptr;
}

bool UHTTPRequestManager::ContainsErrors(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown");
		FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown");

		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Message: %s"), *ErrorMessage);
		return true;
	}
	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString Error = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *Error);
		return true;
	}
	return false;
}

void UHTTPRequestManager::DumpMetaData(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		TSharedPtr<FJsonObject> MetaDataJsonObject = JsonObject->GetObjectField(TEXT("$metadata"));
		FDSMetaData DSMetaData;
		FJsonObjectConverter::JsonObjectToUStruct(MetaDataJsonObject.ToSharedRef(), &DSMetaData);
			
		DSMetaData.Dump();
	}
}

FString UHTTPRequestManager::SerializeJsonObject(const TMap<FString, FString>& Params)
{
	TSharedPtr<FJsonObject> ContentJsonObject = MakeShareable(new FJsonObject);

	for (const auto& Param : Params)
	{
		ContentJsonObject->SetStringField(Param.Key, Param.Value);
	}

	FString Content;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(ContentJsonObject.ToSharedRef(), Writer);
	return Content;
}
