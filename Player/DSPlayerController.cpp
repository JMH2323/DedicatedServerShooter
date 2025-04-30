// Fill out your copyright notice in the Description page of Project Settings.


#include "DSPlayerController.h"

ADSPlayerController::ADSPlayerController()
{
	SingleTripTime = 0.f;
	Username = "";
	PlayerSessionId = "";
}


void ADSPlayerController::Client_SetInputEnabled_Implementation(bool bEnabled)
{
	if (bEnabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}

void ADSPlayerController::Client_TimerUpdated_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	OnTimerUpdated.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADSPlayerController::Client_TimerStopped_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	OnTimerStopped.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADSPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	
	if (GetNetMode() == NM_Standalone) return;
	
	if (IsLocalController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());
	}
}


void ADSPlayerController::Server_Ping_Implementation(float TimeOfRequest)
{
	Client_Pong(TimeOfRequest);
}

void ADSPlayerController::Client_Pong(float TimeOfRequest)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfRequest;
	SingleTripTime = RoundTripTime * 0.5f;
}
