// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "State_BlackFigure_MoveToward.h"

State_BlackFigure_MoveToward::State_BlackFigure_MoveToward(ABaseGhostAIController* _AIComp)
	:State_BlackFigure(_AIComp)
{
}

State_BlackFigure_MoveToward::~State_BlackFigure_MoveToward()
{
}

void State_BlackFigure_MoveToward::Enter()
{
	AIComp->BlackboardComp->SetValueAsName(TEXT("State"), FName("MoveToward"));
}

EGhostState State_BlackFigure_MoveToward::InputHandler()
{
	float Dist = BBComp->GetValueAsFloat(TEXT("TargetDistance"));
	float IdleDist = BBComp->GetValueAsFloat(TEXT("IdleDistance"));
	float AttackDist = BBComp->GetValueAsFloat(TEXT("AttackDistance"));
	bool RunAwayDist = BBComp->GetValueAsBool(TEXT("isRunAway"));
	UObject* target = BBComp->GetValueAsObject(TEXT("Target"));

	if (RunAwayDist)
	{
		return EGhostState::RunAway;
	}

	if (target)
	{
		if (Dist < AttackDist)
		{
			return EGhostState::Attack;
		}
		else if (Dist < IdleDist)
		{
			return EGhostState::MoveToward;
		}
	}
	return EGhostState::Idle;
}

void State_BlackFigure_MoveToward::Exit()
{
	AIComp->BlackboardComp->ClearValue(TEXT("State"));
}
