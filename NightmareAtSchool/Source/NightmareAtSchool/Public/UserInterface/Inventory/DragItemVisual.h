// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

/**
 * 
 */

class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class NIGHTMAREATSCHOOL_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // UI Elements (Widgets) - Declared with BindWidget or manually referenced
    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
    UBorder* ItemBorder;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot", meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot", meta = (BindWidget))
    UTextBlock* ItemQuantity;

};
