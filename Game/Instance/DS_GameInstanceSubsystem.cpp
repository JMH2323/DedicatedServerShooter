// Fill out your copyright notice in the Description page of Project Settings.


#include "DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"


UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
	bGameLiftInitialized = false;
}

void UDS_GameInstanceSubsystem::InitGameLift(const FServerParameters& ServerParams)
{
	if (bGameLiftInitialized) return;

#if WITH_GAMELIFT
	UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Server"));
    	
    FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	GameLiftSdkModule->InitSDK(ServerParams);

	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		FString GameSessionId = FString(gameSession.GetGameSessionId());
		UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
		GameLiftSdkModule->ActivateGameSession();
	};

	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Game Server process is terminating."));
		GameLiftSdkModule->ProcessEnding();
	};

	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	auto OnHealthCheck = []() 
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Performing Health Check"));
		return true;
	};

	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	int32 Port = FURL::UrlConfig.DefaultPort;
	ParseCommandLinePort(Port);

	ProcessParameters.port = Port;

	TArray<FString> LogFiles;
	LogFiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
	ProcessParameters.logParameters = LogFiles;

	UE_LOG(LogDedicatedServers, Log, TEXT("Calling Process Ready."));
	GameLiftSdkModule->ProcessReady(ProcessParameters);
	
#endif
	bGameLiftInitialized = true;
}

void UDS_GameInstanceSubsystem::ParseCommandLinePort(int32& OutPort)
{
	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
	for (const FString& Switch : CommandLineSwitches)
	{
		FString Key;
		FString Value;
		if (Switch.Split("=", &Key, &Value))
		{
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase) && Value.IsNumeric())
			{
				// Lex From String as alternative to "Atoi"
				LexFromString(OutPort, *Value);
				return;
			}
		}
	}
}