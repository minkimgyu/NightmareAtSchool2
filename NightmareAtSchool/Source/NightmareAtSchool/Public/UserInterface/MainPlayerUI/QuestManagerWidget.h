// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestManagerWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UQuestManagerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* BU_Inventory;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Quest;
};
