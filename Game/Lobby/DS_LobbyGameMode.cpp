// Fill out your copyright notice in the Description page of Project Settings.


#include "DS_LobbyGameMode.h"
#include "DedicatedServers/Game/Instance/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "DedicatedServers/Player/DSPlayerController.h"
#include "Kismet/GameplayStatics.h"


ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true;
	LobbyStatus = ELobbyStatus::WaitingForPlayers;
	MinPlayers = 1; // TODO: Change Minimum Players post-testing
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	// Hard Travel
	Super::PostLogin(NewPlayer);
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	// Seamless Travel
	Super::InitSeamlessTravelPlayer(NewController);	
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	CheckAndCancelCountdown();

	// From Game Mode Base
	RemovePlayerSession(Exiting);
	
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// Pulls data from FString Options in Game Sessions Manager.
	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage);
	
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->Username = Username;
	}

	return InitializedString;
	
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username,
                                               FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("Invalid Username or PlayerSessionId");
		return;
	}
#if WITH_GAMELIFT
	
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
	// Change FString to regular char array. Asterisk overload = Tchar -> Convert to ANSI for Gamelift
	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));
	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("DescribePlayerSessionsFailed");
		return;
	}

	const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();
	int32 Count = 0;
	// Get all player sessions
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSessionsFailed");
		return;
	}

	// Check for this session by Username and Reserve Status
	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if(!Username.Equals(PlayerSession.GetPlayerId())) continue;
		
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; Fail PreLogin."), *Username);
			return;
		}

		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ?
			"" : FString::Printf(TEXT("Failed to accept player session for %s"), *Username);
	}
	
#endif
}


void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);

	if (Type == ECountdownTimerType::LobbyCountdown)
	{
		StopCountdownTimer(LobbyCountdownTimer);
		LobbyStatus = ELobbyStatus::SeamlessTravelling;
		TrySeamlessTravel(MapToTravelTo);
	}
	
}

void ADS_LobbyGameMode::CheckAndCancelCountdown()
{
	if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer);
	}
}


void ADS_LobbyGameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		}
	}
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}
	
	//The PID of the running process
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogDedicatedServers, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}