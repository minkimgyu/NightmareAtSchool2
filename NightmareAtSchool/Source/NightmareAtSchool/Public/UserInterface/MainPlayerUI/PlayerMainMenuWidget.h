// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PlayerCharacter.h"
#include "PlayerMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UPlayerMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_MainMenu;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	APlayerCharacter* PlayerChar;

public:
	UFUNCTION()
	void BU_MainMenuClicked();
};
