// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject/DoorBase.h"
#include "PushPullDoor.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UNavModifierComponent;
class ANavBlockOnlyActor;

UCLASS()
class NIGHTMAREATSCHOOL_API APushPullDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APushPullDoor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	// ADoorBase의 핵심 상호작용 로직을 오버라이드합니다.
	virtual void HandleInteraction(IInteractorInterface* Interactor) override;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Navigation")
	class UChildActorComponent* LeftNavBlocker;

	UPROPERTY(VisibleAnywhere, Category = "Navigation")
	class UChildActorComponent* RightNavBlocker;


	/** 3. 문 열리는 소리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Interaction")
	class USoundBase* DoorOpenSound;

	/** 3. 문 열리는 소리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Interaction")
	class USoundBase* DoorCloseSound;




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | Components")
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door | Components")
	UStaticMeshComponent* RightDoorMesh;

	/** 문 닫힘 시 초기 회전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Pull Door | Rotation")
	FRotator ClosedRotationLeft; // 왼쪽 문 초기 회전

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Pull Door | Rotation")
	FRotator ClosedRotationRight; // 오른쪽 문 초기 회전

	/** 문이 완전히 열렸을 때의 목표 각도 (블루프린트에서 설정) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Pull Door | Rotation")
	float OpenAngle = 90.0f; // 90도 회전하여 열리도록 기본 설정

	/** 문 회전의 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Pull Door | Rotation")
	float RotationSpeed = 5.0f; // 회전 보간 속도 (높을수록 빠름)

private:
	/** * 플레이어 캐릭터가 문을 기준으로 앞에 있는지(Push) 뒤에 있는지(Pull) 확인합니다.
	 * @return true: 문 뒤편(Pull 방향), false: 문 앞편(Push 방향)
	 */
	bool IsPlayerBehindDoor(IInteractorInterface* Interactor);

	/** 문 메시를 목표 각도로 부드럽게 회전시키는 함수 */
	void RotateDoor(float DeltaTime);
};
