// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PatrolAIController.generated.h"

/**
 * 
 */
class APatrolManager;
class UBehaviorTree;

UCLASS()
class NIGHTMAREATSCHOOL_API APatrolAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    APatrolAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;

public:
    APatrolManager* GetPatrolManager() const { return PatrolManager; }

private:
    UPROPERTY()
    APatrolManager* PatrolManager;

    UPROPERTY(EditAnywhere)
    UBehaviorTree* PatrolBehaviorTree;
};
