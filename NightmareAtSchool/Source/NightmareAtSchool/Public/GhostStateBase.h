// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABaseGhostAIController;
class UBlackboardComponent;
/**
 * 
 */

//고스트가 쓰일 모든 상태 저장하는 클래스
enum class EGhostState
{
	Idle,
	MoveToward,
	Attack,
	RunAway
};

class NIGHTMAREATSCHOOL_API GhostStateBase
{
public:
	GhostStateBase(ABaseGhostAIController* _AIComp);
	virtual ~GhostStateBase();

	virtual EGhostState InputHandler() = 0;

	virtual void Enter() = 0;

	//virtual void DoAction(float Deltatime) = 0; -> 이건 행동트리에서 알아서 처리해줄테니 실행함수 제외

	virtual void Exit() = 0;

protected:
	ABaseGhostAIController* AIComp;
	UBlackboardComponent* BBComp;
};
