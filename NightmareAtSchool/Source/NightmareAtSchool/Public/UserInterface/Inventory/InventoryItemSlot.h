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

UCLASS()
class NIGHTMAREATSCHOOL_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
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
