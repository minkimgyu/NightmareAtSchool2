// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


// DoorBase.h 상단에 추가 (InteractionInterface.h의 실제 경로로 대체하세요)
#include "InteractionInterface.h"

#include "Valve.generated.h"

// 델리게이트 선언 (매개변수가 없는 형태)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValveOpenedSignature);

// 전방 선언 추가
class IInteractorInterface;
class USceneComponent;
class UStaticMeshComponent;
struct FInteractableData;

UCLASS()
class NIGHTMAREATSCHOOL_API AValve : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AValve();

	// ----------------------------------------------------------------------
	// IInteractionInterface 구현
	// ----------------------------------------------------------------------
	virtual void Interact(IInteractorInterface* Interactor) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	// 블루프린트에서 호출하거나 바인딩할 수 있도록 노출
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnValveOpenedSignature OnValveOpened;

	//// UPROPERTY로 Blueprint에서 설정할 상호작용 데이터를 정의합니다.
	//UPROPERTY(EditInstanceOnly, Category = "Door | Interaction")
	//FInteractableData InteractableData;

protected:

	/** 3. 문 열리는 소리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Interaction")
	class USoundBase* ValveLockSound;


	// ----------------------------------------------------------------------
	// 컴포넌트
	// ----------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valve | Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valve | Components")
	UStaticMeshComponent* ValveMesh;

	/** 문 닫힘 시 초기 회전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve | Rotation")
	FRotator ClosedRotation; // Valve 초기 회전


	/** 문이 완전히 열렸을 때의 목표 각도 (블루프린트에서 설정) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve | Rotation")
	float OpenAngle = 180; // 360도 회전하여 열리도록 기본 설정

	/** 문 회전의 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve | Rotation")
	float RotationSpeed = 2.0f; // 회전 보간 속도 (높을수록 빠름)

	// ----------------------------------------------------------------------
	// 상태 및 상태 전이 함수
	// ----------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valve | State")
	bool bIsLock = false; // 문이 열려있는지 닫혀있는지 상태

	// ----------------------------------------------------------------------
	// 핵심 로직: Valve 열기 처리를 담당하는 함수
	// ----------------------------------------------------------------------
	virtual void HandleInteraction(IInteractorInterface* Interactor);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	/** Valve 메시를 목표 각도로 부드럽게 회전시키는 함수 */
	void RotateValve(float DeltaTime);

};
