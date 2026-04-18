// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainPlayerUI/InMainMenuWidget.h"

#include "Components/Button.h"

#include "PlayerCharacter.h"

void UInMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PC = Cast<APlayerCharacter>(GetOwningPlayer());

	if(BU_Inventory)
		BU_Inventory->OnClicked.AddDynamic(this, &UInMainMenuWidget::OnClicked_InventoryUI);

	if (BU_Quest)
		BU_Quest->OnClicked.AddDynamic(this, &UInMainMenuWidget::OnClicked_QuestUI);

	if (BU_Setting)
		BU_Setting->OnClicked.AddDynamic(this, &UInMainMenuWidget::OnClicked_SettingUI);

	if (BU_Exit)
		BU_Exit->OnClicked.AddDynamic(this, &UInMainMenuWidget::OnClicked_ExitUI);
}

void UInMainMenuWidget::OnClicked_InventoryUI()
{

}

void UInMainMenuWidget::OnClicked_QuestUI()
{
}

void UInMainMenuWidget::OnClicked_SettingUI()
{
}

void UInMainMenuWidget::OnClicked_ExitUI()
{
}


