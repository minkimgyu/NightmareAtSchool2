// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStruct.h"
#include "ItemBase.generated.h"


class APlayerCharacter;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:

    // =========================================================
    // Item Data (아이템 데이터 구조체 복사)
    // =========================================================
    UPROPERTY(VisibleAnywhere, Category = "Item", meta = (UIMin=1, UIMax=100))
    int32 Quantity;

    // =========================================================
    // Owning Inventory (소유 인벤토리 - 나중에 구현)
    // =========================================================
    UPROPERTY(VisibleAnywhere, Category = "Item")
    class UInventoryComponent* OwningInventory; // 나중에 주석 해제

    UPROPERTY(EditAnywhere, Category = "Item")
    FName ID;

    UPROPERTY(EditAnywhere, Category = "Item")
    EItemType ItemType;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    FItemNumericData NumericData;

    UPROPERTY(EditAnywhere, Category = "Item")
    FItemAssetData AssetData;

    UPROPERTY(EditAnywhere, Category = "Item")
    FItemTextData TextData;

    bool bIsCopy;
    bool bIsPickup;

    // =========================================================
    // Public Functions (공개 함수)
    // =========================================================
    UItemBase();

    void ResetItemFlags();

    UFUNCTION(Category = "Item")
    UItemBase* CreateItemCopy() const;

    UFUNCTION(Category = "Item")
    void SetQuantity(const int32 NewQuantity);

    UFUNCTION(Category = "Item")
    void Use(class APlayerCharacter* Character);

    // Getters (FORCEINLINE 최적화
    UFUNCTION(Category = "Item")
    FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; }

    UFUNCTION(Category = "Item")
    FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }

    UFUNCTION(Category = "Item")
    FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; }
protected:
    // =========================================================
    // Operator Overload (연산자 오버로드)
    // =========================================================
    bool operator==(const FName& Key) const { return TextData.ID == Key; }
};
