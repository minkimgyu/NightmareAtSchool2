// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseGhostDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UBaseGhostDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	float MoveSpeed; // 귀신의 이동속도

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	float ViewAngle; // 귀신의 시야각

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	float MoveTowardDistance; // 귀신의 기준 추적거리

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	float AttackDistance; // 귀신의 기준 공격거리
	
};
