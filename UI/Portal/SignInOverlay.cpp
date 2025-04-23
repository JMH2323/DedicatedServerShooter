// Fill out your copyright notice in the Description page of Project Settings.


#include "SignInOverlay.h"
#include "DedicatedServers/UI/API/GameSessions/JoinGame.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "DedicatedServers/UI/Portal/SignIn/SignInPage.h"
#include "DedicatedServers/UI/Portal/SignIn/SignUpPage.h"
#include "DedicatedServers/UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "DedicatedServers/UI/Portal/SignIn/SuccessConfirmedPage.h"
#include "Components/EditableTextBox.h"
#include "HTTP/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(PortalManagerClass);
	check(IsValid(JoinGameWidget));
	check(IsValid(JoinGameWidget->Button_JoinGame));
	
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);

	
	check(Button_SignIn_Test);
	check(Button_SignUp_Test);
	check(Button_ConfirmSignUp_Test);
	check(Button_SuccessConfirmed_Test);

	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::USignInOverlay::ShowSignUpPage);
	Button_ConfirmSignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::USignInOverlay::USignInOverlay::ShowConfirmSignUpPage);
	Button_SuccessConfirmed_Test->OnClicked.AddDynamic(this, &USignInOverlay::USignInOverlay::USignInOverlay::USignInOverlay::ShowSuccessConfirmedPage);


	
}

void USignInOverlay::OnJoinGameButtonClicked()
{
	check(IsValid(PortalManager));
	check(IsValid(JoinGameWidget));
	check(IsValid(JoinGameWidget->Button_JoinGame));
	
	PortalManager->BroadcastJoinGameSessionMessage.AddDynamic(this, &USignInOverlay::UpdateJoinGameStatusMessage);
	PortalManager->JoinGameSession();

	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
}

void USignInOverlay::UpdateJoinGameStatusMessage(const FString& StatusMessage, bool bResetJoinGameButton)
{
	check(IsValid(JoinGameWidget));
	check(IsValid(JoinGameWidget->Button_JoinGame));
	JoinGameWidget->SetStatusMessage(StatusMessage);

	if (bResetJoinGameButton)
	{
		JoinGameWidget->Button_JoinGame->SetIsEnabled(true);
	}
	
}

void USignInOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(ConfirmSignUpPage);
}

void USignInOverlay::ShowSuccessConfirmedPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SuccessConfirmedPage);
}

void USignInOverlay::SignInButtonClicked()
{
	
}

void USignInOverlay::SignUpButtonClicked()
{
	
}

void USignInOverlay::ConfirmButtonClicked()
{
	
}
