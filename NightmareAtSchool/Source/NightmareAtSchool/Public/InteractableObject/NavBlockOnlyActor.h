// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NavBlockOnlyActor.generated.h"

// ✅ Forward Declaration (필수)
class UNavModifierComponent;
class UBoxComponent;

UCLASS()
class NIGHTMAREATSCHOOL_API ANavBlockOnlyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavBlockOnlyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// NavMesh만 막는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNavModifierComponent* NavModifier;

	// 영역 크기 확인용 (에디터에서 보이게)
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;
};
