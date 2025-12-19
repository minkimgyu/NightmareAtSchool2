// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/LineBatchComponent.h"
#include "PatrolPoint.generated.h"

UCLASS()
class NIGHTMAREATSCHOOL_API APatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:
    APatrolPoint();

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float PreferenceWeight = 1.f;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    TArray<APatrolPoint*> ConnectedPoints;

    UPROPERTY()
    ULineBatchComponent* LineBatch;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowConnections = true;


    /** 에디터 버튼: 패트롤 포인트 연결 재생성 */
    UFUNCTION(Category = "Patrol")
    void RefreshConnectionLines();
};
