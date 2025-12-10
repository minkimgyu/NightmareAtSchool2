// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"

void UInventoryTooltip::NativeConstruct()
{
    Super::NativeConstruct();

    const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

    switch (ItemBeingHovered->ItemType)
    {
        case EItemType::Equipment:
            ItemType->SetText(FText::FromString("Equipment"));

            break;
        case EItemType::Quest:
            ItemType->SetText(FText::FromString("Quest"));

            break;
        case EItemType::Consumable:
            ItemType->SetText(FText::FromString("Consumable"));
            break;
        default:
            break;
    }

    // 1. 이름 설정
    ItemName->SetText(ItemBeingHovered->TextData.Name);

    // 4. 사용 텍스트 설정
    UsageText->SetText(ItemBeingHovered->TextData.InteractionText);

    // 5. 설명 설정
    ItemDescription->SetText(ItemBeingHovered->TextData.Description);

    // 1. 아이템 무게 정보 설정
    const FString WeightInfo =
        TEXT("Weight: ") + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight());

    StackWeight->SetText(FText::FromString(WeightInfo));

    // 2. 아이템의 스택 가능 여부에 따른 최대 스택 크기 정보 설정
    // ItemBeingHovered->NumericData.bIsStackable 이 참(true)일 때
    if (ItemBeingHovered->NumericData.bIsStackable)
    {
        // 최대 스택 크기 정보 문자열 생성
        const FString StackInfo =
            TEXT("Max stack size: ") + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize);

        // 최대 스택 크기 UI 요소에 텍스트 설정
        MaxStackSize->SetText(FText::FromString(StackInfo));

        // 이 UI 요소의 가시성을 명시적으로 설정하는 코드가 없지만, 기본적으로 Visible일 수 있습니다.
        // MaxStackSize->SetVisibility(ESlateVisibility::Visible); // 필요하다면 추가
    }
    // ItemBeingHovered->NumericData.bIsStackable 이 거짓(false)일 때 (스택 불가능)
    else
    {
        // 최대 스택 크기 UI 요소를 숨김 (Collapsed: 공간도 차지하지 않음)
        MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
    }
}