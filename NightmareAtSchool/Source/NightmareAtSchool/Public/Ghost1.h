// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ghost1.generated.h"

class UBlackFigureDataAsset;

UCLASS()
class NIGHTMAREATSCHOOL_API AGhost1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGhost1();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Data")
	UBlackFigureDataAsset* BlackFigureData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
