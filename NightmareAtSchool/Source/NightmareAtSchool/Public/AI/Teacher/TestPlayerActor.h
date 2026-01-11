// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestPlayerActor.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UCapsuleComponent;

UCLASS()
class NIGHTMAREATSCHOOL_API ATestPlayerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestPlayerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	UPROPERTY()
	UCapsuleComponent* Capsule;

public:	
};
