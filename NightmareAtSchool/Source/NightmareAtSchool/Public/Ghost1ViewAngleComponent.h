// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterViewAngleComponent.h"
#include "Ghost1ViewAngleComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NIGHTMAREATSCHOOL_API UGhost1ViewAngleComponent : public UMonsterViewAngleComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
