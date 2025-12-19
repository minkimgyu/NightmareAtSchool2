// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolManager.generated.h"

class APatrolPoint;

UCLASS()
class NIGHTMAREATSCHOOL_API APatrolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void CollectPatrolPoints();
    void GenerateConnections(); // MST
    void RefreshAllConnectionLines();

    void BuildPatrolConnections();

public:	
    UPROPERTY(EditAnywhere, Category = "Patrol|AutoConnect")
    bool bAutoGenerateConnections = true;

    UPROPERTY(EditAnywhere, Category = "Patrol|AutoConnect")
    bool bBidirectional = true;



    /** 에디터 버튼: 패트롤 포인트 연결 재생성 */
    UFUNCTION(CallInEditor, Category = "Patrol|Editor")
    void RebuildPatrolConnections();


    /** 월드에 존재하는 모든 패트롤 포인트 */
    UPROPERTY(VisibleAnywhere, Category = "Patrol")
    TArray<APatrolPoint*> AllPatrolPoints;

    /** 초기 포인트 하나 반환 */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    APatrolPoint* GetRandomStartPoint() const;

    /** 현재 포인트 기준 다음 포인트 선택 */
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    APatrolPoint* GetNextPatrolPoint(APatrolPoint* CurrentPoint) const;
};
