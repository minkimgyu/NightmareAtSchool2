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

 /** 메뉴 상태 변경을 알리기 위한 델리게이트 선언 (파라미터: bool bIsVisible) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuVisibilityChanged, bool, bIsVisible);

class UMainMenu;
class UInteractionWidget;
class USprintBar;
class UPlayerHP;

struct FInteractableData;

UCLASS()
class NIGHTMAREATSCHOOL_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/** 블루프린트에서 바인딩할 수 있는 델리게이트 인스턴스 */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMenuVisibilityChanged OnMenuVisibilityChanged;

	// =========================================================
	// 프로퍼티, 변수
	// =========================================================

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")

	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USprintBar> USprintBarClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerHP> HPWidgetClass;

	bool bIsMenuVisible;

	AMainHUD();

	// 캐릭터에서 데미지를 입었을 때 컨트롤러의 이 함수를 호출하게 합니다.
	void UpdateHPUI(float CurrentHP);

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

	UPROPERTY()
	UPlayerHP* HPWidget;

	virtual void BeginPlay() override;
};
