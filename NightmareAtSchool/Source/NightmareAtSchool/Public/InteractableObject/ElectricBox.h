// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// DoorBase.h 상단에 추가 (InteractionInterface.h의 실제 경로로 대체하세요)
#include "InteractionInterface.h"

#include "ElectricBox.generated.h"

// 델리게이트 선언 (매개변수가 없는 형태)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInstallElectricBoxSignature);

// 전방 선언 추가
class IInteractorInterface;
class USceneComponent;
class UStaticMeshComponent;
struct FInteractableData;

UCLASS()
class NIGHTMAREATSCHOOL_API AElectricBox : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectricBox();

	// ----------------------------------------------------------------------
	// IInteractionInterface 구현
	// ----------------------------------------------------------------------
	virtual void Interact(IInteractorInterface* Interactor) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	// 블루프린트에서 호출하거나 바인딩할 수 있도록 노출
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInstallElectricBoxSignature OnElectricBoxInstalled;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 3. 문 열리는 소리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Interaction")
	class USoundBase* BoxInstallSound;

	// ----------------------------------------------------------------------
	// 컴포넌트
	// ----------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ElectricBox | Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ElectricBox | Components")
	UStaticMeshComponent* WallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ElectricBox | Components")
	UStaticMeshComponent* BoxMesh;


	/** 문을 열기 위해 인벤토리에 필요한 아이템의 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricBox | Requirements")
	FName RequiredItemID;

	/** 문을 열기 위해 인벤토리에 필요한 아이템의 최소 수량 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricBox | Requirements", meta = (ClampMin = "1"))
	int32 RequiredItemAmount = 1;

	// ----------------------------------------------------------------------
	// 상태 및 상태 전이 함수
	// ----------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ElectricBox | State")
	bool bIsPlanted = false; // 문이 열려있는지 닫혀있는지 상태

	// ----------------------------------------------------------------------
	// 핵심 로직: Box 설치를 담당하는 함수
	// ----------------------------------------------------------------------
	virtual void HandleInteraction(IInteractorInterface* Interactor);

	bool CanInstallElectricBox(IInteractorInterface* Interactor) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/** Valve 메시를 목표 각도로 부드럽게 회전시키는 함수 */
	void PlantBox();

};
