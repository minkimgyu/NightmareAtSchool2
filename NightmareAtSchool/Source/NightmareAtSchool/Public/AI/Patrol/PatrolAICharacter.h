// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InteractorInterface.h"
#include "Flashlight/ULightDetectable.h"

#include "PatrolAICharacter.generated.h"

class UInteractionComponent;
class UInventoryComponent;
class IULightDetectable;
struct FInteractableData; // 구조체 전방 선언 추가

UCLASS()
class NIGHTMAREATSCHOOL_API APatrolAICharacter : public ACharacter, public IInteractorInterface, public IULightDetectable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APatrolAICharacter();


	// IULightDetectable 구현
	virtual void SetIlluminated(bool bState) override;

	// BT에서 참조하거나 외부에서 확인하기 위한 Getter
	UFUNCTION(BlueprintCallable, Category = "AI|State")
	bool IsIlluminated() const { return bIsIlluminated; }



	// IInteractorInterface 구현 (정확한 시그니처 일치 필요)
	virtual AActor* GetInteractorActor() override { return this; }

	// 로그에서 경고가 났던 부분: const 유무를 인터페이스와 일치시켜야 함
	virtual UInventoryComponent* GetInventory() const override { return nullptr; }

	// PlayerCharacter.h 에 선언
	virtual void UpdateInteractionWidget(FInteractableData* InteractableData) override {}

protected:
	// 손전등에 비춰지고 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	bool bIsIlluminated;

	// 블랙보드 키 이름 (에디터에서 설정하거나 상수로 관리)
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IlluminatedKeyName = TEXT("IsIlluminated");


	// 상호작용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	USceneComponent* InteractionSight;

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
