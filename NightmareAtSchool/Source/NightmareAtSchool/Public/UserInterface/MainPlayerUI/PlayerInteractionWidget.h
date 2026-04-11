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

	//UFUNCTION()
	//void BU_RunClicked();


	// 기존 BU_RunClicked는 제거하거나 주석 처리하고 아래 두 개를 추가합니다.
	UFUNCTION()
	void BU_RunPressed();

	UFUNCTION()
	void BU_RunReleased();

	// 상태 동기화를 위해 Tick을 오버라이드하는 것이 좋습니다.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


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
