// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

/**
 * 
 */

class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
class UBorder;
class UImage;
class UTextBlock;

// 1. 델리게이트 선언 (아이템 데이터를 파라미터로 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotClicked, UItemBase*, ItemData);

UCLASS()
class NIGHTMAREATSCHOOL_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
    // 2. 외부(InventoryPanel)에서 접근 가능한 델리게이트 변수
    UPROPERTY(BlueprintAssignable, Category = "Inventory Slot")
    FOnInventorySlotClicked OnSlotClicked;

    // FORCEINLINE functions for setting and getting the item reference
    FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; }
    FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }

protected:
    // TSubclassOf variables to hold references to Blueprint classes
    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
    TSubclassOf<UDragItemVisual> DragItemVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
    TSubclassOf<UInventoryTooltip> TooltipClass;

    // The actual item data reference
    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
    UItemBase* ItemReference;

    // UI Elements (Widgets) - Declared with BindWidget or manually referenced
    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
    UBorder* ItemBorder;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot", meta=(BindWidget))
    UImage* ItemIcon;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot", meta = (BindWidget))
    UTextBlock* ItemQuantity;



    // --- Virtual Functions for UI Event Handling (from image_17e95c.jpg) ---
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
