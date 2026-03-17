// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DistanceToTargetComponent.generated.h"

//class AAIController;
//class UBlackboardComponent;
class UMonsterViewAngleComponent;
//
////델리게이트 선언
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam
//(
//	FOnTargetDist, // 델리게이트 타입 이름
//	float,  // 파라미터 타입
//	TargetDist // 이름
//);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UDistanceToTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDistanceToTargetComponent();

	//시야각에서 발견한 타겟 설정용
	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable)
	float GetDistanceToTarget() const;

	UPROPERTY()
	AActor* Target;

	//UPROPERTY(BlueprintAssignable)
	//FOnTargetDist OnTargetDist;

	//UPROPERTY()
	//float TargetPos;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APawn* OwnerPawn;
	//AAIController* AIController;
	//UBlackboardComponent* Blackboard;
	float TargetAndGhostDist;

	UMonsterViewAngleComponent* MonsterViewAngleComp;

	void MonsterViewAngleEvent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//void SaveBlackboard();
	//void GetAIController();
		
};
