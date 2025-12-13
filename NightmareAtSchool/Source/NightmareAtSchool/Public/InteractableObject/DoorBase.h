// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// DoorBase.h 상단에 추가 (InteractionInterface.h의 실제 경로로 대체하세요)
#include "InteractionInterface.h"

#include "DoorBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class APlayerCharacter;
struct FInteractableData;

UCLASS()
class NIGHTMAREATSCHOOL_API ADoorBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorBase();

	// ----------------------------------------------------------------------
	// IInteractionInterface 구현
	// ----------------------------------------------------------------------
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	//// UPROPERTY로 Blueprint에서 설정할 상호작용 데이터를 정의합니다.
	//UPROPERTY(EditInstanceOnly, Category = "Door | Interaction")
	//FInteractableData InteractableData;

protected:
	// ----------------------------------------------------------------------
	// 컴포넌트
	// ----------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | Components")
	UStaticMeshComponent* DoorFrameMesh;

	// ----------------------------------------------------------------------
	// 상태 및 상태 전이 함수
	// ----------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | State")
	bool bIsOpen; // 문이 열려있는지 닫혀있는지 상태

	//// 문 열기/닫기 로직 (Blueprint에서 애니메이션을 처리)
	//UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	//void OpenDoor(const FVector& InteractorLocation); // 문을 여는 Blueprint 이벤트

	//UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	//void CloseDoor(); // 문을 닫는 Blueprint 이벤트


	// ----------------------------------------------------------------------
	// 핵심 로직: 문 열기/닫기 처리를 담당하는 함수
	// ----------------------------------------------------------------------
	virtual void HandleInteraction(APlayerCharacter* PlayerCharacter);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
