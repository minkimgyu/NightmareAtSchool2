// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Sprint/SprintBar.h"
#include "UserInterface/HP/PlayerHP.h"

#include "InteractionInterface.h" // APlayerCharacter.h에서 사용되던 인터페이스

AMainHUD::AMainHUD()
{
}

void AMainHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::UpdateHPUI(float CurrentHP)
{
	HPWidget->UpdateHPText(CurrentHP);
}

void AMainHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AMainHUD::ShowSprintBar()
{
	if (SprintBar)
	{
		SprintBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideSprintBar()
{
	if (SprintBar)
	{
		SprintBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::UpdateInteractionWidget(const  FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5); // zorder
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1); // zorder
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (USprintBarClass)
	{
		SprintBar = CreateWidget<USprintBar>(GetWorld(), USprintBarClass);
		SprintBar->AddToViewport(-1); // zorder
		SprintBar->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (HPWidgetClass)
	{
		HPWidget = CreateWidget<UPlayerHP>(GetWorld(), HPWidgetClass);
		HPWidget->AddToViewport(-1); // zorder
		HPWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
