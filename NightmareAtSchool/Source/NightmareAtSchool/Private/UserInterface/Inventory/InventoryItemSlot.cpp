// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "UserInterface/Inventory/DragItemVisual.h"


// InventoryItemSlot.cpp 파일 상단
#include "UserInterface/Inventory/ItemDragDropOperation.h" // UItemDragDropOperation에 대한 정의

#include "Items/ItemBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UInventoryItemSlot::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (TooltipClass)
    {
        UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
        Tooltip->InventorySlotBeingHovered = this; // This line is commented out in the image
        SetToolTip(Tooltip);
    }
}

void UInventoryItemSlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (ItemReference)
    {
        //switch (ItemReference->ItemQuality)
        //{
        //case EItemQuality::Shoddy:
        //    ItemBorder->SetBrushColor(FLinearColor::Gray);
        //    break;
        //case EItemQuality::Common:
        //    ItemBorder->SetBrushColor(FLinearColor::White);
        //    break;
        //case EItemQuality::Quality:
        //    ItemBorder->SetBrushColor(FLinearColor::White);
        //    break;
        //case EItemQuality::Masterwork:
        //    ItemBorder->SetBrushColor(FLinearColor::White);
        //    break;
        //case EItemQuality::Grandmaster:
        //    // Assuming R, G, B, A in the format shown
        //    ItemBorder->SetBrushColor(FLinearColor(100.0f, 65.0f, 0.0f, 1.0f)); // orange
        //    break;
        //default:
        //    break;
        //}

        ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

        if (ItemReference->NumericData.bIsStackable)
        {
            ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
        }
        else
        {
            ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 1. 부모 클래스의 기본 구현을 먼저 호출합니다.
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // 2. 마우스 왼쪽 버튼 클릭 여부를 확인합니다.
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // 왼쪽 버튼이 눌렸다면, 드래그 작업을 감지하도록 설정하고 Handled 상태를 반환합니다.
        // DetectDragIfPressed는 LeftMouseButton이 해제되지 않고 움직임이 감지될 때 드래그 작업을 시작합니다.
        return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }

    // 3. 마우스 오른쪽 버튼 클릭 (서브메뉴 로직)
    // if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    // {
    //     // 서브메뉴를 팝업하는 로직이 여기에 들어갑니다.
    //     // 예: return Reply.Handled();
    // }

    // sub-menu on right click will happen here

    // 4. 왼쪽/오른쪽 버튼 외의 모든 이벤트는 처리되지 않았음(Unhandle)을 반환합니다.
    return Reply.Unhandled();
}
void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    if (DragItemVisualClass)
    {
        // 드래그 비주얼 위젯 생성
        const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);

        // 드래그 비주얼의 아이콘 설정
        DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

        // 드래그 비주얼의 테두리 색상 설정
        DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

        // 드래그 비주얼의 수량 텍스트 설정
        DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

        // 드래그 작업(Operation) 생성
        UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();

        // 드래그 작업에 아이템 정보 및 소스 인벤토리 설정
        DragItemOperation->SourceItem = ItemReference;
        DragItemOperation->SourceInventory = ItemReference->OwningInventory;

        // 드래그 작업에 기본 드래그 비주얼 및 피벗 설정
        DragItemOperation->DefaultDragVisual = DragVisual;
        DragItemOperation->Pivot = EDragPivot::TopLeft;

        // 출력 오퍼레이션에 설정
        OutOperation = DragItemOperation;
    }
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
