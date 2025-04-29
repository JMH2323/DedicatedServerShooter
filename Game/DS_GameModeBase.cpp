// Fill out your copyright notice in the Description page of Project Settings.


#include "DS_GameModeBase.h"

void ADS_GameModeBase::StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.TimerFinishedDelegate.BindWeakLambda(this,[]()
	{
		
	});
}

void ADS_GameModeBase::StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	
}

void ADS_GameModeBase::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	
}
