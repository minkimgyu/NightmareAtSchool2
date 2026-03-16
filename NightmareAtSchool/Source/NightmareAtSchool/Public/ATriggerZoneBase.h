// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATriggerZoneBase.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class NIGHTMAREATSCHOOL_API AATriggerZoneBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATriggerZoneBase();

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	AActor* TargetActor; // 움직일 대상 설정 -> 블루프린트에서 설정가능

	//충돌했을 때 호출할 함수
	UFUNCTION()
	void OnTriggerOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintImplementableEvent, Category="Trigger")
	void OnTriggered();

	bool IsCol;

public:	

};
