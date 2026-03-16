// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "State_BlackFigure.h"
#include "CoreMinimal.h"

/**
 * 
 */

//검은 형체의 상태 관리
class NIGHTMAREATSCHOOL_API State_BlackFigure_Idle : public State_BlackFigure
{
public:
	State_BlackFigure_Idle(ABaseGhostAIController* _AIComp);
	virtual ~State_BlackFigure_Idle();

	virtual EGhostState InputHandler() override;

	virtual void Enter() override;

	//virtual void DoAction(float Deltatime) = 0; -> 이건 행동트리에서 알아서 처리해줄테니 실행함수 제외

	virtual void Exit() override;
};
