// Fill out your copyright notice in the Description page of Project Settings.


#include "SignInOverlay.h"

#include "HTTP/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(PortalManagerClass);
	PortalManager = NewObject<UPortalManager>(PortalManagerClass);
	
}
