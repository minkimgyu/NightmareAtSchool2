// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "InventoryPanel.generated.h"

class UInventoryItemSlot;
class APlayerCharacter;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RefreshInventory();


	// --- 추가된 UI 요소 (중앙 상세 창) ---
	// 위젯 블루프린트의 이름과 정확히 일치해야 합니다.

	// 1. 헤더에 컨테이너 추가
	UPROPERTY(meta = (BindWidget))
	class UWidget* DetailWindowContainer; // 이미지, 이름, 설명을 모두 포함하는 부모 위젯

	UPROPERTY(meta = (BindWidget))
	UImage* ItemLargeIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

	// UI Elements (Widgets)
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* InventoryVerticalBox;

	/*UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;*/

	// Gameplay References
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	// Class Reference for spawning/creating inventory slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

protected:
	void SetInfoText() const;

	// 슬롯 클릭 시 호출될 함수 (UFUNCTION 필수)
	UFUNCTION()
	void UpdateDetailWindow(UItemBase* ItemData);

	virtual void NativeOnInitialized() override;

	virtual bool NativeOnDrop(
		const FGeometry& InGeometry, 
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation
	) override;
};
