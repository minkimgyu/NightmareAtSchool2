// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterViewAngleBase.h"
#include "MonsterViewAngleComponent.generated.h"

class AAIController;
class UBlackboardComponent;
class USceneComponent;

//델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam
(
	FOnTarget, // 델리게이트 타입 이름
	AActor*,  // 파라미터 타입
	Target // //파라미터 이름
);

USTRUCT(BlueprintType)
struct FPlayerScanGruop
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* PlayerActor; // 플레이어

	UPROPERTY()
	TArray<USceneComponent*> ScanPoints; // 스캔할 지점들 저장

	bool operator==(const FPlayerScanGruop& Other) const
	{
		return PlayerActor == Other.PlayerActor;
	}

	// 2. 해시 함수 (TSet 사용 시 필수)
	friend uint32 GetTypeHash(const FPlayerScanGruop& Other)
	{
		// PlayerActor의 주소값을 해시 키로 사용합니다.
		return GetTypeHash(Other.PlayerActor);
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NIGHTMAREATSCHOOL_API UMonsterViewAngleComponent : public UMonsterViewAngleBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMonsterViewAngleComponent();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewAngle")
	float ViewDistance = 1000.0f; // 인지 범위 -> 임시 데이터임

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewAngle", meta = (ClampMin = "0.0", ClampMax = "180.0")) // 0~180도 사이만 조정가능
	float ViewAngle = 140.0f; // 시야각 -> 마찬가지 임시 데이터 수정하고싶다면 DATA클래스로 가시오.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	TSet<FPlayerScanGruop> NearPlayerArray; // 가까운 플레이어들을 모아둔 변수

	UPROPERTY(BlueprintReadOnly, Category = "Target")
	AActor* FoundPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
	AActor* NewPlayer; //새롭게 발견한 플레이어

	UPROPERTY(BlueprintAssignable)
	FOnTarget OnTarget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual AActor* CheckFindTarget() const override; // 실제 시야에 들어온 플레이어 찾는 함수

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called monsterLocation Read Only
	FVector GetMonsterLocation() const;
	FTimerHandle ViewCheckTimerHandle;

private:
	FVector MonsterLocation;
	TArray<FPlayerScanGruop> PlayerScanArray; // 게임 시작후 모든 플레이어를 찾는 변수
	FVector MonsterHeadPos;
	FVector MonsterMiddlePos;
	FVector MonsterFeetPos;

	APawn* OwnerPawn;
	AAIController* AIController;
	UBlackboardComponent* Blackboard;

private:
	//Write function

	virtual bool IsInAngleTarget(AActor* _player) const; // 타겟이 시야에 들어왔는지 확인하는 함수
	virtual bool IsObstacle(FPlayerScanGruop _player) const; // 장애물 존재여부
	virtual void CheckMonsterLocation(); // 단순 몬스터의 위치
	virtual void CheckNearPlayer(); // 가까이 다가온 플레이어 찾는 함수
	virtual void MonsterViewExe();
	float CalAngle(float _cos) const; // 단순 각도 계산
	//void GetAIController();
	//void SaveBlackboard();
	void DrawViewAngle();
	void HandleTargetChange();
};
