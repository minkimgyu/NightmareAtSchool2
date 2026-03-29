// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// 1. 아이템 ID를 전달하기 위한 델리게이트 선언 (FName 매개변수 포함)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedToInventory, FName, ItemID);

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : 
		ActualAmountAdded(0), 
		OperationResult(EItemAddResult::IAR_NoItemAdded), 
		ResultMessage(FText::GetEmpty())
	{
	}

	// Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// 2. 블루프린트에서 호출 및 할당 가능한 델리게이트 변수 추가
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemAddedToInventory OnItemAdded;

	// PROPERTIES & VARIABLES
	// UInventoryComponent 클래스 public 섹션 내
	//UPROPERTY(BlueprintAssignable, Category = "Inventory") // <-- UPROPERTY 추가
	FOnInventoryUpdated OnInventoryUpdated;

	// FUNCTIONS
	UInventoryComponent();

	FItemAddResult HandleAddItem(UItemBase* InputItem);

	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	void RemoveSingleInstanceOfItem(UItemBase* ItemIn);
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	bool RemoveItemByID(const FName ItemID, const int32 AmountToRemove);

	// getters
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; };
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };

	// setters
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };

	int32 GetItemAmountByID(const FName ItemID) const;

protected:

	// PROPERTIES & VARIABLES

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	float InventoryTotalWeight;

	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;

	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	// FUNCTIONS
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FItemAddResult HandleNonStackableItems(UItemBase* ItemBase);
	int32 HandleStackableItems(UItemBase* ItemBase, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* ItemBase, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* ExistingItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);
};
