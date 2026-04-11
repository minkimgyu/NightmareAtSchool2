// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Sprint/SprintBar.h"
#include "UserInterface/HP/PlayerHP.h"

#include "UserInterface/Joystick/JoystickWidget.h"
#include "UserInterface/MainPlayerUI/PlayerInteractionWidget.h"
#include "UserInterface/MainPlayerUI/PlayerMainMenuWidget.h"
#include "UserInterface/Joystick/BackGroundTouchWidget.h"

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

		// ⭐ 델리게이트 호출 (true 전달)
		if (OnMenuVisibilityChanged.IsBound())
		{
			OnMenuVisibilityChanged.Broadcast(bIsMenuVisible);
		}
	}
}

void AMainHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

		// ⭐ 델리게이트 호출 (false 전달)
		if (OnMenuVisibilityChanged.IsBound())
		{
			OnMenuVisibilityChanged.Broadcast(bIsMenuVisible);
		}
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

	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		// 1. 마우스 커서를 강제로 보이게 설정
		PC->bShowMouseCursor = true;

		// 2. 게임 조작과 UI 조작을 동시에 허용하는 모드로 변경
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false); // 마우스를 눌러도 커서가 사라지지 않게 함

		PC->SetInputMode(InputMode);
	}

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

	if (BackGroundTouchWidgetClass)
	{
		BackGroundTouchWidget = CreateWidget<UBackGroundTouchWidget>(GetWorld(), BackGroundTouchWidgetClass);

		if (BackGroundTouchWidget)
			BackGroundTouchWidget->AddToViewport(-1); // 우선순위 다른 메인UI와 같게 설정
	}

	if (USprintBarClass)
	{
		SprintBar = CreateWidget<USprintBar>(GetWorld(), USprintBarClass);
		SprintBar->AddToViewport(-2); // zorder
		SprintBar->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (JoystickWidgetClass)
	{
		JoystickWidget = CreateWidget<UJoystickWidget>(GetWorld(), JoystickWidgetClass);

		if (JoystickWidget)
			JoystickWidget->AddToViewport(-1); // 우선순위 다른 메인UI와 같게 설정

		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPawn());

		if (Player)
		{
			// 3. 캐릭터의 포인터 변수에 방금 만든 위젯 주소를 넣어줌!
			Player->JoystickWidgetPtr = JoystickWidget;
		}
	}

	if (PlayerInteractionClass)
	{
		PlayerInteraction = CreateWidget<UPlayerInteractionWidget>(GetWorld(), PlayerInteractionClass);

		if (PlayerInteraction)
			PlayerInteraction->AddToViewport(-1); // 우선순위 다른 메인UI와 같게 설정
	}

	if (PlayerMainMenuClass)
	{
		PlayerMainMenu = CreateWidget<UPlayerMainMenuWidget>(GetWorld(), PlayerMainMenuClass);

		if (PlayerMainMenu)
			PlayerMainMenu->AddToViewport(-1); // 우선순위 다른 메인UI와 같게 설정
	}

	if (HPWidgetClass)
	{
		HPWidget = CreateWidget<UPlayerHP>(GetWorld(), HPWidgetClass);
		HPWidget->AddToViewport(-2); // zorder
		HPWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
