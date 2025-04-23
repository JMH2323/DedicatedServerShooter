// Fill out your copyright notice in the Description page of Project Settings.


#include "DS_GameMode.h"


DEFINE_LOG_CATEGORY(LogDS_GameMode);

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();
#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void ADS_GameMode::InitGameLift()
{
	UE_LOG(LogDS_GameMode, Log, TEXT("Initializing the GameLift Server"));

	//Getting the module first.
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	//Define the server parameters for an Amazon GameLift Servers Anywhere fleet. These are not needed for an Amazon GameLift Servers managed EC2 fleet.
	FServerParameters ServerParameters;

	SetServerParameters(ServerParameters);
	
	FString glProcessId = "";
	if (FParse::Value(FCommandLine::Get(), TEXT("-processId="), glProcessId))
	{
		ServerParameters.m_processId = TCHAR_TO_UTF8(*glProcessId);
	}
	else
	{
		// If no ProcessId is passed as a command line argument, generate a randomized unique string.
		FString TimeString = FString::FromInt(std::time(nullptr));
		FString ProcessId = "ProcessId_" + TimeString;
		ServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
	}

	//The PID of the running process
	UE_LOG(LogDS_GameMode, Log, TEXT("PID: %s"), *ServerParameters.m_processId);

	//InitSDK establishes a local connection with the Amazon GameLift Servers Agent to enable further communication.
	//Use InitSDK(serverParameters) for an Amazon GameLift Servers Anywhere fleet. 
	//Use InitSDK() for Amazon GameLift Servers managed EC2 fleet.
	GameLiftSdkModule->InitSDK(ServerParameters);
	//GameLiftSdkModule->InitSDK();
	
	//Implement callback function onStartGameSession
	//Amazon GameLift Servers sends a game session activation request to the game server
	//and passes a game session object with game properties and other settings.
	//Here is where a game server takes action based on the game session object.
	//When the game server is ready to receive incoming player connections, 
	//it invokes the server SDK call ActivateGameSession().
	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		FString GameSessionId = FString(gameSession.GetGameSessionId());
		UE_LOG(LogDS_GameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
		GameLiftSdkModule->ActivateGameSession();
	};
	
	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	//Implement callback function OnProcessTerminate
	//Amazon GameLift Servers invokes this callback before shutting down the instance hosting this game server.
	//It gives the game server a chance to save its state, communicate with services, etc., 
	//and initiate shut down. When the game server is ready to shut down, it invokes the 
	//server SDK call ProcessEnding() to tell Amazon GameLift Servers it is shutting down.
	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Game Server Process is terminating"));
		GameLiftSdkModule->ProcessEnding();
	};

	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);
	
	//Implement callback function OnHealthCheck
	//Amazon GameLift Servers invokes this callback approximately every 60 seconds.
	//A game server might want to check the health of dependencies, etc.
	//Then it returns health status true if healthy, false otherwise.
	//The game server must respond within 60 seconds, or Amazon GameLift Servers records 'false'.
	//In this example, the game server always reports healthy.
	auto OnHealthCheck = []()
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Performing Health Check"));
		return true;
	};

	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	//The game server gets ready to report that it is ready to host game sessions
	//and that it will listen on port for incoming player connections.
	int32 Port = FURL::UrlConfig.DefaultPort;

	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;	
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
	for (const FString& Switch : CommandLineSwitches)
	{
		FString Key;
		FString Value;
		if (Switch.Split("=", &Key, &Value))
		{
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
			{
				Port = FCString::Atoi(*Value);
				break;
			}
		}
	}
	
	ProcessParameters.port = Port;

	//Here, the game server tells Amazon GameLift Servers where to find game session log files.
	//At the end of a game session, Amazon GameLift Servers uploads everything in the specified 
	//location and stores it in the cloud for access later.
	TArray<FString> LogFiles;
	LogFiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
	ProcessParameters.logParameters = LogFiles;

	//The game server calls ProcessReady() to tell Amazon GameLift Servers it's ready to host game sessions.
	UE_LOG(LogDS_GameMode, Log, TEXT("Calling Process Ready"));
	GameLiftSdkModule->ProcessReady(ProcessParameters);
}

void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the Amazon GameLift Servers Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}
}
