// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMenu.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"
#include "PlayerCharacter.h"
#include "Items/ItemBase.h"

void UMainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // Try to cast the generic UDragDropOperation to the specific UItemDragDropOperation
    const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

    // Check if the cast was successful (meaning it is an item drag/drop operation)
    // and check if the PlayerCharacter is valid and the SourceItem exists
    if (ItemDragDrop && PlayerCharacter && ItemDragDrop->SourceItem)
    {
        // Execute the drop logic on the PlayerCharacter
        // This line attempts to add the dropped item back to the player's inventory
        PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);

        // Return true to signify that the drop operation was handled
        return true;
    }

    // Return false if the operation was not an item drop or was not handled
    return false;
}