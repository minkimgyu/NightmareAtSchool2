// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryTooltip.generated.h"

/**
 * 
 */

class UTextBlock;
class UInventoryItemSlot;

UCLASS()
class NIGHTMAREATSCHOOL_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnyWhere)
    UInventoryItemSlot* InventorySlotBeingHovered;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemType;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UsageText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescription;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaxStackSize;

    //UPROPERTY(meta = (BindWidget))
    //UTextBlock* StackSizeValue;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* StackWeight;

protected:

    virtual void NativeConstruct() override;
};
