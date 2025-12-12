// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InteractionInterface.h"
#include "UserInterface/Interaction/InteractionWidget.h"

#include "MainHUD.generated.h"

/**
 * 
 */

class UMainMenu;
class UInteractionWidget;
class USprintBar;

struct FInteractableData;

UCLASS()
class NIGHTMAREATSCHOOL_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// =========================================================
	// 프로퍼티, 변수
	// =========================================================

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")

	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USprintBar> USprintBarClass;

	bool bIsMenuVisible;

	AMainHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowSprintBar();
	void HideSprintBar();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const  FInteractableData* InteractableData) const;

protected:

	UPROPERTY()

	UMainMenu* MainMenuWidget;

	UPROPERTY()

	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	USprintBar* SprintBar;

	virtual void BeginPlay() override;
};
