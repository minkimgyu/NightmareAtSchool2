// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "PlayerInteractionWidget.generated.h"

class APlayerCharacter;
class UInteractionComponent;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UPlayerInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	APlayerCharacter* PlayerChar;

protected:

	virtual void NativeConstruct() override;

	//----------버튼---------
	UPROPERTY(meta = (BindWidget))
	UButton* BU_PickUp;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_SitDown;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Run;

	UPROPERTY(meta = (BindWidget))
	UButton* BU_Light;
	//=========================

	//------------이미지---------
	UPROPERTY(meta = (BindWidget))
	UImage* IM_SitDown;

	UPROPERTY(meta = (BindWidget))
	UImage* IM_Run;

	UPROPERTY(meta = (BindWidget))
	UImage* IM_Light;
	//-----------------------------

protected:

	UFUNCTION()
	void BU_PickUpClicked();

	UFUNCTION()
	void BU_SitDownClicked();

	UFUNCTION()
	void BU_RunClicked();

	UFUNCTION()
	void BU_LightClicked();

private:

	bool isOnFlashLight = false;
	bool isRun = false;
	bool isSit = false;

	UInteractionComponent* InteractionComp;
	
private:

	void InitUIOpacity();
};
