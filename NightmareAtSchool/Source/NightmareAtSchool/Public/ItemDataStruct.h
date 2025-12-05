#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStruct.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Consumable UMETA(DisplayName = "Consumable"),
    Equipment UMETA(DisplayName = "Equipment"),
    Quest UMETA(DisplayName = "Quest")
};

USTRUCT(BlueprintType)
struct FItemTextData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Item Text")
    FName ID; // 아이템의 고유 ID. 데이터 테이블 검색 키로 사용됩니다.

    UPROPERTY(EditAnywhere, Category = "Item Text")
    FText Name;

    UPROPERTY(EditAnywhere, Category = "Item Text")
    FText Description;

    UPROPERTY(EditAnywhere, Category = "Item Text")
    FText InteractionText;
};

// 아이템 수치 데이터 (Item Numeric Data)
USTRUCT(BlueprintType)
struct FItemNumericData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = "Item Numeric")
    int32 MaxStackSize;

    UPROPERTY(EditAnywhere, Category = "Item Numeric")
    float Weight;

    UPROPERTY(EditAnywhere, Category = "Item Numeric")
    bool bStackable;
};

USTRUCT(BlueprintType)
struct FItemAssetData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere)
    UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FName ID;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    int32 Quantity;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemNumericData NumericData;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemAssetData AssetData;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemTextData TextData;
};