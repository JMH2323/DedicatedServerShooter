// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DedicatedServers/UI/HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"

struct FDSRecordMatchStatsInput;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	void RecordMatchStats(const FDSRecordMatchStatsInput RecordMatchStatsInput);
	
	
};
