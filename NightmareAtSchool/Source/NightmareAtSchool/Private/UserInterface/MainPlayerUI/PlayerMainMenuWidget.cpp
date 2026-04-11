// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainPlayerUI/PlayerMainMenuWidget.h"

void UPlayerMainMenuWidget::NativeConstruct()
{
	PlayerChar = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerChar)
		return;

	if (BU_MainMenu)
	{
		BU_MainMenu->OnClicked.AddDynamic(this, &UPlayerMainMenuWidget::BU_MainMenuClicked);
	}
}

void UPlayerMainMenuWidget::BU_MainMenuClicked()
{
	PlayerChar->ToggleMenu();
}
