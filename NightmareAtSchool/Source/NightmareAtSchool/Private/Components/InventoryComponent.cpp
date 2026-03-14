// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    // ...

}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
    if (ItemIn)
    {
        if (InventoryContents.Contains(ItemIn))
        {
            return ItemIn;
        }
    }
    return nullptr;
}


UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
    if (ItemIn)
    {
        if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
        {
            return *Result;
        }
    }
    return nullptr;
}

int32 UInventoryComponent::GetItemAmountByID(const FName ItemID) const
{
    if (ItemID == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetItemAmountByID called with NAME_None ID. Returning 0."));
        return 0;
    }

    int32 TotalAmount = 0;

    // 인벤토리의 모든 아이템을 순회합니다.
    for (const UItemBase* Item : InventoryContents)
    {
        if (Item)
        {
            // 내 인벤토리에 지금 뭐가 들어있는지 전부 찍어보기
            UE_LOG(LogTemp, Warning, TEXT("In Inventory - Item ID: %s, Qty: %d"), *Item->ID.ToString(), Item->Quantity);

            // 아이템의 ID와 입력된 ItemID가 일치하는지 확인합니다.
            if (Item->ID == ItemID)
            {
                // 일치하면 해당 아이템 스택의 수량(Quantity)을 총 수량에 더합니다.
                TotalAmount += Item->Quantity;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Total amount of Item ID '%s' found: %d"), *ItemID.ToString(), TotalAmount);

    return TotalAmount;
}


UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
    // 함수 호출 시작 시 로그
    UE_LOG(LogTemp, Warning, TEXT("--- FindNextPartialStack Start (Tracing Logic Flow) ---"));
    // 찾으려는 아이템 ID 출력
    UE_LOG(LogTemp, Warning, TEXT("Target ID: %s"), *ItemIn->ID.ToString());

    if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate(
        [&ItemIn](const UItemBase* InventoryItem)->bool
    {
        // 1. 비교 조건: ID 일치 (FName은 '==' 비교가 적절함)
        const bool bIsSameID = InventoryItem->ID == ItemIn->ID;

        // 2. 비교 조건: 부분 스택 여부
        const bool bIsNotFull = !InventoryItem->IsFullItemStack();


        // --- 검증 로그 ---
        // 인벤토리 아이템의 ID를 출력하여 Target ID와 비교
        UE_LOG(LogTemp, Log, TEXT("  Checking Slot ID: %s, Quantity: %d, Is Full: %s"),
            *InventoryItem->ID.ToString(),
            InventoryItem->Quantity,
            InventoryItem->IsFullItemStack() ? TEXT("TRUE") : TEXT("FALSE"));


        if (!bIsSameID)
        {
            UE_LOG(LogTemp, Warning, TEXT("    -> Result: FALSE (Reason: ID Mismatch!)"));
            return false;
        }

        if (!bIsNotFull)
        {
            UE_LOG(LogTemp, Log, TEXT("    -> Result: FALSE (Reason: Full Stack)"));
            return false;
        }
        // -------------

        // 두 조건 모두 만족
        UE_LOG(LogTemp, Warning, TEXT("    -> Result: TRUE (MATCH FOUND)"));
        return true;
    }))
    {
        UE_LOG(LogTemp, Warning, TEXT("--- FindNextPartialStack Success: Match Found ---"));
        return *Result;
    }

    UE_LOG(LogTemp, Warning, TEXT("--- FindNextPartialStack Failed: No Partial Stack Found ---"));
    return nullptr;
}


int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
    // Image 5 (Upper part)
    const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
    if (WeightMaxAddAmount >= RequestedAddAmount)
    {
        return RequestedAddAmount;
    }

    return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
    // Image 5 (Lower part)
    const int32 AmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;
    return FMath::Min(InitialRequestedAddAmount, AmountToMakeFullStack);
}



void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemIn)
{
    // Note: The function signature in the .cpp (Image b00471) is 'UItemBase* ItemToRemove', 
    // but the declaration (Image b09220) is 'UItemBase* ItemIn'. Using 'ItemToRemove' as in the implementation.

    InventoryContents.RemoveSingle(ItemIn); // Assumes ItemIn is passed as the item to remove
    OnInventoryUpdated.Broadcast(); // Delegate call assumed to exist
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
    const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

    ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

    InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

    OnInventoryUpdated.Broadcast(); // Delegate call assumed to exist

    return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
    if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
    {
        RemoveAmountOfItem(ItemIn, AmountToSplit);
        AddNewItem(ItemIn, AmountToSplit);
    }
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

bool UInventoryComponent::RemoveItemByID(const FName ItemID, const int32 AmountToRemove)
{
    if (ItemID == NAME_None || AmountToRemove <= 0) return false;

    // 1. 총 수량 선제 확인 (원자성 보장)
    int32 TotalOwned = GetItemAmountByID(ItemID);
    if (TotalOwned < AmountToRemove)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItemByID: Not enough items. (Owned: %d, Required: %d)"), TotalOwned, AmountToRemove);
        return false;
    }

    int32 RemainingToRemove = AmountToRemove;

    // 2. 인벤토리 순회 및 차감
    // 뒤에서부터 순회하는 것이 삭제 시 가장 안전합니다.
    for (int32 i = InventoryContents.Num() - 1; i >= 0; --i)
    {
        UItemBase* Item = InventoryContents[i];

        // 유효성 검사 추가
        if (!IsValid(Item) || Item->ID != ItemID) continue;

        int32 AmountInStack = Item->Quantity;
        int32 CanRemoveFromThisStack = FMath::Min(RemainingToRemove, AmountInStack);

        // 실제 수량 차감
        Item->SetQuantity(Item->Quantity - CanRemoveFromThisStack);
        InventoryTotalWeight -= (CanRemoveFromThisStack * Item->GetItemSingleWeight());
        RemainingToRemove -= CanRemoveFromThisStack;

        //// 수량이 0이 된 아이템은 배열에서 완전히 제거
        //if (Item->Quantity <= 0)
        //{
        //    InventoryContents.RemoveAt(i);
        //}

        // 3. 목표 수량을 모두 제거했다면 루프를 즉시 종료하고 성공 리턴
        if (RemainingToRemove <= 0)
        {
            OnInventoryUpdated.Broadcast();
            UE_LOG(LogTemp, Log, TEXT("Successfully removed %d of Item ID: %s"), AmountToRemove, *ItemID.ToString());
            return true;
        }
    }

    // 만약 루프가 끝났는데 수량을 다 못 채웠다면 (이론상 위에서 체크해서 발생하면 안 됨)
    OnInventoryUpdated.Broadcast();
    return RemainingToRemove <= 0;
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
    // Image af229b / af1f8f
    // check if the input item has valid weight
    if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
    {
        return FItemAddResult::AddedNone(FText::Format(
            FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->TextData.Name));
    }

    // will the item weight overflow weight capacity
    if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
    {
        return FItemAddResult::AddedNone(FText::Format(
            FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
    }

    // adding one more item would overflow slot capacity
    if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
    {
        return FItemAddResult::AddedNone(FText::Format(
            FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
    }

    AddNewItem(InputItem, 1);
    // return added all result (Implicitly assumed, as there is no specific return line)

    return FItemAddResult::AddedAll(1, FText::Format(
        FText::FromString("Successfully added {0} to the inventory."), InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
    if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
    {
        // invalid item data
        return 0;
    }

    int32 AmountToDistribute = RequestedAddAmount;

    // check if the input item already exists in the inventory and is not a full stack
    UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

    // distribute item stack over existing stacks
    while (ExistingItemStack)
    {
        // calculate how many of the existing item would be needed to make the next full stack
        const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);

        // calculate how many of the AmountToMakeFullStack can actually be carried based on weight capacity
        const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);


        UE_LOG(LogTemp, Warning, TEXT("AmountToMakeFullStack : %d"), AmountToMakeFullStack);
        UE_LOG(LogTemp, Warning, TEXT("WeightLimitAddAmount : %d"), WeightLimitAddAmount);

        // as long as the remaining amount of the item does not overflow weight capacity
        if (WeightLimitAddAmount > 0)
        {
            // adjust the existing items stack quantity and inventory total weight
            ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
            InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

            // adjust the count to be distributed
            AmountToDistribute -= WeightLimitAddAmount;
            ItemIn->SetQuantity(AmountToDistribute);

            // if max weight capacity would be exceeded by another item, just return early
            if (InventoryTotalWeight >= InventoryWeightCapacity)
            {
                OnInventoryUpdated.Broadcast();
                return RequestedAddAmount - AmountToDistribute;
            }
        }
        else if (WeightLimitAddAmount <= 0)
        {
            if (AmountToDistribute != RequestedAddAmount)
            {
                OnInventoryUpdated.Broadcast();
                return RequestedAddAmount - AmountToDistribute;
            }

            return 0;
        }

        if (AmountToDistribute <= 0)
        {
            OnInventoryUpdated.Broadcast();
            return RequestedAddAmount;
        }

        ExistingItemStack = FindNextPartialStack(ItemIn);
    }

    UE_LOG(LogTemp, Warning, TEXT("reached first"));

    // no more partial stacks found, check if a new stack can be added
    if ((InventoryContents.Num() + 1) <= InventorySlotsCapacity)
    {
        // attempt to add as many from the remaining item quantity that can fit inventory weight capacity
        const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

        if (WeightLimitAddAmount > 0)
        {
            // if there is still more item to distribute, but weight limit has been reached
            if (WeightLimitAddAmount < AmountToDistribute)
            {
                // adjust the input item and add a new stack with as many as can be held
                AmountToDistribute -= WeightLimitAddAmount;
                ItemIn->SetQuantity(AmountToDistribute);

                // create a copy since only a partial stack is being added
                AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
                return RequestedAddAmount - AmountToDistribute;
            }
            // otherwise, the full remainder of the stack can be added
            AddNewItem(ItemIn, AmountToDistribute);
            return RequestedAddAmount;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("reached end"));

    // reached if there is free item slots, but no remaining weight capacity
    OnInventoryUpdated.Broadcast();
    return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
    // Image af261e
    if (GetOwner())
    {
        const int32 InitialRequestedAddAmount = InputItem->Quantity;

        // handle non-stackable items
        if (!InputItem->NumericData.bIsStackable)
        {  
            return HandleNonStackableItems(InputItem);
        }

        
        // handle stackable
        const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);


        UE_LOG(LogTemp, Warning, TEXT("Stackable Amount Added: %d %d"), StackableAmountAdded, InitialRequestedAddAmount);

        if (StackableAmountAdded == InitialRequestedAddAmount)
        {
            // return added all result 
            return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
                FText::FromString("Successfully added {0} {1} to the inventory."),
                InitialRequestedAddAmount,
                InputItem->TextData.Name));
        }

        if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
        {
            // return added partial result
                        // return added all result 
            return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
                FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
                InputItem->TextData.Name,
                StackableAmountAdded));
        }

        if (StackableAmountAdded <= 0)
        {
            return FItemAddResult::AddedNone(FText::Format(
                FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
                InputItem->TextData.Name));
            // return added none result
        }
    }

    check(false);
    // Final return if GetOwner() fails or logic falls through
    return FItemAddResult::AddedNone(FText::FromString("Totally Failed!"));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
    // Image af1754
    UItemBase* NewItem;

    // 카피 되거나 픽업 된 경우
    if (Item->bIsCopy || Item->bIsPickup)
    {
        // if the item is already a copy, or is a world pickup
        NewItem = Item;
        NewItem->ResetItemFlags();
    }
    else // 아닌 경우는 카피를 만든다.
    {
        // used when splitting or dragging to/from another inventory
        NewItem = Item->CreateItemCopy();
    }

    NewItem->OwningInventory = this;
    NewItem->SetQuantity(AmountToAdd);

    InventoryContents.Add(NewItem);
    InventoryTotalWeight += NewItem->GetItemStackWeight();
    OnInventoryUpdated.Broadcast();
}