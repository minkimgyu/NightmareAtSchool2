// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionInterface.h"

#include "InterfaceTestActor.generated.h"

class IInteractorInterface;

UCLASS()
class NIGHTMAREATSCHOOL_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInterfaceTestActor();

protected:
	/** 상호작용 가능한 객체의 유형입니다. */
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	/** 상호작용 데이터입니다. */
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	FInteractableData InstanceInteractableData;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	virtual void BeginInteract() override;
	virtual void EndInteract() override;

	virtual void Interact(IInteractorInterface* Interactor) override;
};
