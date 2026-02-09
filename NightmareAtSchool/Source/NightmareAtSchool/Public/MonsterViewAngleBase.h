// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterViewAngleBase.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UMonsterViewAngleBase : public UActorComponent
{
	GENERATED_BODY()

public:
	//시야각 구현에 있어서 타겟을 찾는 것은 무조건 구현
	virtual AActor* CheckFindTarget() const
		PURE_VIRTUAL(UMonsterViewAngleBase::CheckFindTarget, return nullptr;);

		
};
