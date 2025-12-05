// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemBase.h" // UItemBase*를 사용하기 위해 필요하다고 가정합니다.
#include "InteractionInterface.h"

#include "Pickup.generated.h"

class UDataTable;
class UItemBase;

UCLASS()
class NIGHTMAREATSCHOOL_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
   
	// FUNCTIONS
	// Sets default values for this actor's properties
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:
	// PROPERTIES & VARIABLES

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference; // 커서가 위치한 부분

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;


    // FUNCTIONS
    virtual void BeginPlay() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override; // [00:04:21]

	void UpdateInteractableData();

	void TakePickup(const APlayerCharacter* PlayerCharacter);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif 
};
