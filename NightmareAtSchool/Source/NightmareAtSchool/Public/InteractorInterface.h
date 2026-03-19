// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/InventoryComponent.h"
#include "InteractorInterface.generated.h"

class UInventoryComponent;
struct FInteractableData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NIGHTMAREATSCHOOL_API IInteractorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 인벤토리 컴포넌트를 반환합니다. 
	// 기본적으로 nullptr을 반환하게 두면, 인벤토리가 없는 AI는 이 함수를 굳이 오버라이드할 필요가 없습니다.


	// 상속받는 클래스에서 'return this;'만 하면 되도록 순수 가상 함수로 만듭니다.
	virtual AActor* GetInteractorActor() = 0;
	virtual UInventoryComponent* GetInventory() const = 0;
	virtual void UpdateInteractionWidget(FInteractableData* InteractableData) = 0;
};
