// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UAttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	// 애니메이션에서 노티파이 시점에 실행될 함수
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
