// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UGhostAIEventHubCompoent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UUGhostAIEventHubCompoent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUGhostAIEventHubCompoent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	//MonsterViewComponent의 이벤트용
		
};
