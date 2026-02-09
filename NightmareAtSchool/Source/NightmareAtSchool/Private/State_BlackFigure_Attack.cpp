// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "State_BlackFigure_Attack.h"

State_BlackFigure_Attack::State_BlackFigure_Attack(ABaseGhostAIController* _AIComp)
	:State_BlackFigure(_AIComp)
{
}

State_BlackFigure_Attack::~State_BlackFigure_Attack()
{
}

void State_BlackFigure_Attack::Enter()
{
	AIComp->BlackboardComp->SetValueAsName(TEXT("State"), FName("Attack"));
}

EGhostState State_BlackFigure_Attack::InputHandler()
{
	float Dist = BBComp->GetValueAsFloat(TEXT("TargetDistance"));
	float IdleDist = BBComp->GetValueAsFloat(TEXT("IdleDistance"));
	float AttackDist = BBComp->GetValueAsFloat(TEXT("AttackDistance"));
	UObject* target = BBComp->GetValueAsObject(TEXT("Target"));
	bool RunAwayDist = BBComp->GetValueAsBool(TEXT("isRunAway"));

	if (RunAwayDist)
	{
		return EGhostState::RunAway;
	}

	if (!target)
	{
		return EGhostState::Idle;
	}
	else if (Dist < AttackDist)
	{
		return EGhostState::Attack;
	}
	else
	{
		return EGhostState::MoveToward;
	}
}

void State_BlackFigure_Attack::Exit()
{
	AIComp->BlackboardComp->ClearValue(TEXT("State"));
}
