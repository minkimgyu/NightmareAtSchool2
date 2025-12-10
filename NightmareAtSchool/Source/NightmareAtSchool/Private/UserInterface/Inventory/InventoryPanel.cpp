// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "UserInterface/Inventory/InventoryPanel.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::RefreshInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("Refresh Inventory!"));

    // Check if references are valid before proceeding
    if (InventoryReference && InventorySlotClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Can Refresh Inventory!"));

        // 1. Clear all existing slots in the UI panel
        InventoryWarpBox->ClearChildren();

        // 2. Iterate through all items in the inventory
        for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
        {
            // 3. Create a new inventory slot widget
            UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);

            // NOTE: The line below is commented out in the image, but is necessary to set the item data.
            ItemSlot->SetItemReference(InventoryItem);

            // 4. Add the new slot to the UI panel (WrapBox)
            InventoryWarpBox->AddChildToWrapBox(ItemSlot);
        }

        SetInfoText();
    }
}

void UInventoryPanel::SetInfoText() const
{
    // 무게 정보 문자열을 생성합니다.
    // 형식: 현재 총 무게 / 최대 무게 용량
    const FString WeightInfoValue
    {
        FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + L"/" +
        FString::SanitizeFloat(InventoryReference->GetWeightCapacity())
    };

    // 용량(슬롯 수) 정보 문자열을 생성합니다.
    // 형식: 현재 아이템 수 / 최대 슬롯 용량
    const FString CapacityInfoValue
    {
        FString::FromInt(InventoryReference->GetInventoryContents().Num()) + L"/" +
        FString::FromInt(InventoryReference->GetSlotsCapacity())
    };

    // 생성된 문자열을 UI 텍스트 필드에 설정합니다.
    WeightInfo->SetText(FText::FromString(WeightInfoValue));
    CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

void UInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // Get the Player Character and Inventory Reference
    PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        InventoryReference = PlayerCharacter->GetInventory();
        if (InventoryReference)
        {
            // Bind RefreshInventory to the inventory update delegate
            InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);

            SetInfoText();
        }
    }
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // UDragDropOperation*를 ItemDragDropOperation*으로 안전하게 캐스팅합니다.
    const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

    // 캐스팅이 성공하고 (ItemDragDrop이 유효하며), 
    // 드래그된 아이템의 SourceItem과 InventoryReference가 유효한지 확인합니다.
    if (ItemDragDrop->SourceItem && InventoryReference)
    {
        // 인벤토리 패널에 아이템 드롭이 감지되었음을 로그로 출력합니다.
        UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."));

        // // return true를 반환하면 이 위젯에서 드롭 작업을 멈춥니다. 
        // // (즉, 드롭 작업을 소비하고 다른 위젯으로 전달되지 않도록 합니다.)
        return true;
    }

    // // return false를 반환하면 드롭 작업이 아래에 있는 (있다면) 위젯들로 전달됩니다.
    // // (즉, 드롭 작업을 소비하지 않고 다른 위젯도 처리할 기회를 줍니다.)
    return false;
}
