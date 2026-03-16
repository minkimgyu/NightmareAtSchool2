// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePatrolManager.generated.h"

class APatrolPoint;

UCLASS()
class NIGHTMAREATSCHOOL_API ABasePatrolManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePatrolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** 초기 포인트 하나 반환 */
	//UFUNCTION(BlueprintCallable, Category = "Patrol")
	virtual APatrolPoint* GetRandomStartPoint() PURE_VIRTUAL(ABasePatrolManager::GetRandomStartPoint, return nullptr;);

	/** 현재 포인트 기준 다음 포인트 선택 */
	//UFUNCTION(BlueprintCallable, Category = "Patrol")
	virtual APatrolPoint* GetNextPatrolPoint(APatrolPoint* CurrentPoint) PURE_VIRTUAL(ABasePatrolManager::GetNextPatrolPoint, return nullptr;);
};