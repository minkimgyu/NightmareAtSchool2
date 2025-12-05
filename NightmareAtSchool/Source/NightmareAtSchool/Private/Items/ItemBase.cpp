// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

UItemBase::UItemBase()
{
	// 생성자: 데이터 테이블에서 로드될 예정이므로 현재는 비워둡니다.
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(GetTransientPackage(), StaticClass());

	// 모든 데이터를 새로 생성된 객체에 복사합니다.
	ItemCopy->Quantity = Quantity;
	ItemCopy->TextData = TextData;
	ItemCopy->NumericData = NumericData;
	ItemCopy->AssetData = AssetData;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		// FMath::Clamp를 사용하여 수량을 0과 최대 스택 크기 사이로 제한합니다.
		// Stackable(스택 가능) 여부에 따라 최대 스택 크기(MaxStackSize) 또는 1로 제한합니다.
		Quantity = FMath::Clamp(NewQuantity,
			0,
			(NumericData.bStackable ? NumericData.MaxStackSize : 1));

		// =========================================================
		// Inventory Management (나중에 구현될 인벤토리 관리 로직)
		// =========================================================
		/* if (IsValid(OwningInventory))
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveItem(this); // 슈도코드
			}
		}
		*/
	}
}

void UItemBase::Use(APlayerCharacter* Character)
{
	// 아이템 사용 기능 (현재는 비어있음)
}