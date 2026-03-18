// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ULightDetectable.h"
#include "TestLightDetectableActor.generated.h"

UCLASS()
class NIGHTMAREATSCHOOL_API ATestLightDetectableActor : public AActor, public IULightDetectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestLightDetectableActor();

	// 인터페이스 구현 (override 필수)
	virtual void SetIlluminated(bool bState) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	bool bIsIlluminated = false;
};
