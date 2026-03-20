// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject/DoorBase.h"

#include "SlidingDoor.generated.h"

class IInteractorInterface;

UCLASS()
class NIGHTMAREATSCHOOL_API ASlidingDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// ----------------------------------------------------------------------
	// 슬라이딩 컴포넌트 및 변수
	// ----------------------------------------------------------------------
	/** 문이 열려야 할 최종 위치를 나타내는 컴포넌트입니다. (블루프린트에서 배치) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Door")
	USceneComponent* OpenPositionMarkerRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Door")
	USceneComponent* OpenPositionMarkerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | Components")
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | Components")
	UStaticMeshComponent* RightDoorMesh;


	/** 3. 문 열리는 소리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Interaction")
	class USoundBase* DoorOpenSound;


	/** 문 닫힘 위치 (ADoorBase::DoorMesh의 초기 위치) */
	FVector ClosedLocationLeft;
	FVector ClosedLocationRight;

	/** 문 움직임의 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	float SlideSpeed = 5.0f; // 보간 속도 (높을수록 빠름)

	// ----------------------------------------------------------------------
	// 오버라이드 함수
	// ----------------------------------------------------------------------
	virtual void BeginPlay() override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	// ADoorBase에서 Blueprint 이벤트를 오버라이드하여 C++ 로직으로 대체합니다.
	// 더 이상 BlueprintImplementableEvent가 필요하지 않으므로, ADoorBase에서 Blueprint 이벤트 선언을 제거하고,
	// 이곳에서 C++ 함수로 재정의하거나, ADoorBase의 HandleInteraction을 직접 수정할 수 있습니다.

	// 여기서는 ADoorBase의 OpenDoor/CloseDoor Blueprint 이벤트 대신,
	// ADoorBase::HandleInteraction을 오버라이드하여 C++ 로직을 사용하겠습니다.
	virtual void HandleInteraction(IInteractorInterface* Interactor) override;

private:
	// ----------------------------------------------------------------------
	// 슬라이딩 로직
	// ----------------------------------------------------------------------

	/** 문 메시를 목표 위치로 부드럽게 이동시키는 함수 */
	void SlideDoor(float DeltaTime);
};
