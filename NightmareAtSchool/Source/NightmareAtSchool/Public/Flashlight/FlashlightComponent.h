// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h" // SceneComponent로 변경
#include "Components/SphereComponent.h" // 콜라이더 추가
#include "Components/SpotLightComponent.h" // 라이트 컴포넌트 추가
#include "ULightDetectable.h"
#include "FlashlightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UFlashlightComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlashlightComponent();

	// 손전등 전원 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight Settings")
	bool bIsOn = true;

	// 블루프린트에서 호출 가능한 전원 제어 함수
	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void ToggleFlashlight();

	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void SetFlashlightState(bool bNewState);

private:
	// 상태 업데이트 로직 (UpdateLightSettings 내부에서 처리하거나 따로 분리)
	void ApplyPowerState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateLightSettings(); // 라이트 수치 동기화 함수

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 컴포넌트를 초기화하기 위해 캐릭터가 호출해 줄 함수
	void InitializeFlashlight(USpotLightComponent* InLight, USphereComponent* InSphere);

	// 직접 생성하지 않고 포인터만 들고 있습니다.
	UPROPERTY()
	USpotLightComponent* FlashlightMesh123;

	UPROPERTY()
	USphereComponent* DetectionSphere123;

	// 디버그 시각화 여부 (에디터에서 체크박스로 조절 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight Settings")
	bool bShowDebug = true;

	// 감지 설정 값
	UPROPERTY(EditAnywhere, Category = "Flashlight Settings")
	float DetectionAngle = 30.0f; // 손전등의 반각(Half Angle)

	UPROPERTY(EditAnywhere, Category = "Flashlight Settings")
	float DetectionDistance = 1000.0f;

private:
	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	// 충돌 컴포넌트 이벤트 연결용
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
