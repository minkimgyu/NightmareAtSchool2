// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InteractorInterface.h"

#include "PatrolAICharacter.generated.h"

class UInteractionComponent;
class UInventoryComponent;
struct FInteractableData; // 구조체 전방 선언 추가

UCLASS()
class NIGHTMAREATSCHOOL_API APatrolAICharacter : public ACharacter, public IInteractorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APatrolAICharacter();

	// IInteractorInterface 구현 (정확한 시그니처 일치 필요)
	virtual AActor* GetInteractorActor() override { return this; }

	// 로그에서 경고가 났던 부분: const 유무를 인터페이스와 일치시켜야 함
	virtual UInventoryComponent* GetInventory() const override { return nullptr; }

	// PlayerCharacter.h 에 선언
	virtual void UpdateInteractionWidget(FInteractableData* InteractableData) override {}

protected:
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
