// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InMainMenuWidget.generated.h"

class UButton;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UInMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Inventory;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Quest;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Setting;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Exit;

	APlayerCharacter* PC;
	
private:

	void OnClicked_InventoryUI();
	void OnClicked_QuestUI();
	void OnClicked_SettingUI();
	void OnClicked_ExitUI();
	
};
