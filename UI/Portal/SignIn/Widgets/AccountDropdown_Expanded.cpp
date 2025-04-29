// Fill out your copyright notice in the Description page of Project Settings.


#include "AccountDropdown_Expanded.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DedicatedServers/Player/DSLocalPlayerSubsystem.h"
#include "DedicatedServers/UI/Portal/HTTP/PortalManager.h"

void UAccountDropdown_Expanded::NativeConstruct()
{
	Super::NativeConstruct();

	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	Button_SignOut->OnClicked.AddDynamic(this, &UAccountDropdown_Expanded::SignOutButton_OnClicked);
	Button_SignOut->OnHovered.AddDynamic(this, &UAccountDropdown_Expanded::SignOutButton_Hover);
	Button_SignOut->OnUnhovered.AddDynamic(this, &UAccountDropdown_Expanded::SignOutButton_Unhover);

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		TextBlock_Email->SetText(FText::FromString(LocalPlayerSubsystem->Email));
	}	
}

void UAccountDropdown_Expanded::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetSignOutButtonStyleTransparent();
	SignOutButton_Unhover();
	
}

void UAccountDropdown_Expanded::SignOutButton_Hover()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UAccountDropdown_Expanded::SignOutButton_Unhover()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(UnhoveredTextColor);
}

void UAccountDropdown_Expanded::SignOutButton_OnClicked()
{
	Button_SignOut->SetIsEnabled(false);
	
	check(PortalManager);
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		FDSAuthenticationResult AuthenticationResult = LocalPlayerSubsystem->GetAuthResult();
		PortalManager->SignOut(AuthenticationResult.AccessToken);
	}
}

void UAccountDropdown_Expanded::SetSignOutButtonStyleTransparent()
{
	FButtonStyle Style;
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.f ));
	Style.Disabled = Brush;
	Style.Hovered = Brush;
	Style.Pressed = Brush;
	Style.Normal = Brush;
	Button_SignOut->SetStyle(Style);
}

UDSLocalPlayerSubsystem* UAccountDropdown_Expanded::GetLocalPlayerSubsystem()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem))
		{
			return LocalPlayerSubsystem;
		}
	}
	return nullptr;
}
