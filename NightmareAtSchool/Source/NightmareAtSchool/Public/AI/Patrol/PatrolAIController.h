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
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
struct FAIStimulus;

UCLASS()
class NIGHTMAREATSCHOOL_API APatrolAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    APatrolAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus); // 매개변수 수정

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    APatrolManager* GetPatrolManager() const { return PatrolManager; }

    float GetPatrolSpeed() const { return PatrolSpeed; }
    float GetInvestigateSpeed() const { return InvestigateSpeed; }

private:
    void DrawSightView(FVector Center, FVector Forward, FColor Color, float Range);
    void DrawHearingView(FVector Center);

    // ... 기존 코드 아래 추가
protected:
    UPROPERTY(EditAnywhere, Category = "AI|Speed")
    float PatrolSpeed = 300.f;

    UPROPERTY(EditAnywhere, Category = "AI|Speed")
    float InvestigateSpeed = 600.f;

    UPROPERTY(EditAnywhere, Category = "AI|Speed")
    float ChasingSpeed = 800.f;

    // AI 감지 설정 변수화
    UPROPERTY(EditAnywhere, Category = "AI|Sight")
    float SightRadius = 800.f;

    UPROPERTY(EditAnywhere, Category = "AI|Sight")
    float LoseSightRadius = 1000.f;

    UPROPERTY(EditAnywhere, Category = "AI|Sight")
    float SightAngle = 360; // 이제 360도이므로 원형으로 그려집니다.

    UPROPERTY(EditAnywhere, Category = "AI|Hearing")
    float HearingRange = 1500.f;

private:
    UPROPERTY()
    APatrolManager* PatrolManager;

    UPROPERTY(EditAnywhere)
    UBehaviorTree* PatrolBehaviorTree;

    UPROPERTY() // 중요
    UAISenseConfig_Hearing* HearingConfig;

    UPROPERTY() // 중요
    UAISenseConfig_Sight* SightConfig;
};
