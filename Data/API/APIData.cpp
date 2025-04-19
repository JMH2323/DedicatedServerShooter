// Fill out your copyright notice in the Description page of Project Settings.


#include "APIData.h"

FString UAPIData::GetAPIEndpoint(const FGameplayTag& APIEndpoint)
{
	FString ResourceName = Resources.FindChecked(APIEndpoint);
	return InvokeURL + "/" + Stage + "/" + ResourceName;
}
