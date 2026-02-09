// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterViewAngleComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NIGHTMAREATSCHOOL_API UMonsterViewAngleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMonsterViewAngleComponent();

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "ViewAngle")
	float ViewDistance = 1000.0f; // 인지 범위

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "ViewAngle", meta = (ClampMin = "0.0", ClampMax = "180.0")) // 0~180도 사이만 조정가능
		float ViewAngle = 140.0f; // 시야각

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "ViewAngle")
	TSet<AActor*> NearPlayerArray; // 가까운 플레이어들을 모아둔 변수

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called monsterLocation Read Only
	FVector GetMonsterLocation() const;

private:
	FVector MonsterLocation;
	TArray<AActor*> PlayerArray; // 게임 시작후 모든 플레이어를 찾는 변수
	FVector MonsterHeadPos;
	FVector MonsterMiddlePos;
	FVector MonsterFeetPos;

private:
	//Write function
	void CheckMonsterLocation();
	void CheckNearPlayer(); // 가까이 다가온 플레이어 찾는 함수
	AActor* CheckFindPlayer(); // 실제 시야에 들어온 플레이어 찾는 함수
	float CalAngle(float _cos);
	bool IsInAnglePlayer(AActor* _player);
	bool IsObstacle(AActor* _player); // 장애물 존재여부
};
