// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInterface/Inventory/InventoryPanel.h"

#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

#include "Items/ItemBase.h" // 프로젝트 내 실제 ItemBase.h 경로에 맞춰 수정하세요.

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
        InventoryVerticalBox->ClearChildren();

        const TArray<UItemBase*>& Contents = InventoryReference->GetInventoryContents();
        if (Contents.Num() > 0)
        {
            // 2. Iterate through all items in the inventory
            for (UItemBase* const& InventoryItem : Contents)
            {
                // 3. Create a new inventory slot widget
                UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);

                // --- 추가된 로직: 슬롯 클릭 이벤트 바인딩 ---
                    // 슬롯 클래스에 OnSlotClicked 델리게이트가 정의되어 있어야 합니다.
                ItemSlot->OnSlotClicked.AddDynamic(this, &UInventoryPanel::UpdateDetailWindow);

                // NOTE: The line below is commented out in the image, but is necessary to set the item data.
                ItemSlot->SetItemReference(InventoryItem);

                // 4. Add the new slot to the UI panel (WrapBox)
                InventoryVerticalBox->AddChild(ItemSlot);
            }

            UpdateDetailWindow(Contents[0]); // 첫 번째 아이템 정보를 기본으로 출력
        }
        else
        {
            // 아이템이 하나도 없다면 nullptr을 전달하여 상세 창을 숨깁니다.
            UpdateDetailWindow(nullptr);
        }

        SetInfoText();
    }
}

// 상세 정보 창 업데이트 로직
void UInventoryPanel::UpdateDetailWindow(UItemBase* ItemData)
{
    // 아이템 데이터가 유효할 때만 정보를 표시하고 가시성을 켭니다.
    if (ItemData)
    {
        DetailWindowContainer->SetVisibility(ESlateVisibility::Visible);

        // 1. 큰 이미지 및 텍스트 설정
        ItemLargeIcon->SetBrushFromTexture(ItemData->AssetData.Icon);
        ItemNameText->SetText(ItemData->TextData.Name);
        ItemDescriptionText->SetText(ItemData->TextData.Description);

        // 2. 가시성을 Visible로 설정

        // 만약 구분선(Line)이 있다면 그것도 여기서 Visible 처리하세요.
    }
    else
    {
        // 부모를 통째로 숨겨버리면 자식들이 개별적으로 안 꺼지는 문제가 해결됩니다.
        DetailWindowContainer->SetVisibility(ESlateVisibility::Collapsed);

        // 아이템 데이터가 없을 경우 모든 상세 위젯을 숨깁니다.
        // Collapsed는 공간까지 차지하지 않도록 숨기는 설정입니다.
    }
}

void UInventoryPanel::SetInfoText() const
{
    // 무게 정보 문자열을 생성합니다.
    // 형식: 현재 총 무게 / 최대 무게 용량
    //const FString WeightInfoValue
    //{
    //    FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + L"/" +
    //    FString::SanitizeFloat(InventoryReference->GetWeightCapacity())
    //};

    //// 용량(슬롯 수) 정보 문자열을 생성합니다.
    //// 형식: 현재 아이템 수 / 최대 슬롯 용량
    //const FString CapacityInfoValue
    //{
    //    FString::FromInt(InventoryReference->GetInventoryContents().Num()) + L"/" +
    //    FString::FromInt(InventoryReference->GetSlotsCapacity())
    //};

    //// 생성된 문자열을 UI 텍스트 필드에 설정합니다.
    //WeightInfo->SetText(FText::FromString(WeightInfoValue));
    //CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
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

    RefreshInventory();
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
