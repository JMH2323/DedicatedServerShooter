// Fill out your copyright notice in the Description page of Project Settings.


#include "SignInPage.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USignInPage::UpdateStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_SignIn->SetIsEnabled(true);
	}
}