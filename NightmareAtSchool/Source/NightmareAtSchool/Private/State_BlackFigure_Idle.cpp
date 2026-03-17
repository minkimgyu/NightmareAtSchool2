// Fill out your copyright notice in the Description page of Project Settings.


#include "State_BlackFigure_Idle.h"
#include "BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

State_BlackFigure_Idle::State_BlackFigure_Idle(ABaseGhostAIController* _AIComp)
	:State_BlackFigure(_AIComp)
{
}

State_BlackFigure_Idle::~State_BlackFigure_Idle()
{
}

void State_BlackFigure_Idle::Enter()
{
	AIComp->BlackboardComp->SetValueAsName(TEXT("State"), FName("Idle"));
}

EGhostState State_BlackFigure_Idle::InputHandler()
{
	float Dist = BBComp->GetValueAsFloat(TEXT("TargetDistance"));
	float IdleDist= BBComp->GetValueAsFloat(TEXT("IdleDistance"));
	float AttackDist = BBComp->GetValueAsFloat(TEXT("AttackDistance"));
	UObject* target = BBComp->GetValueAsObject(TEXT("Target"));

	
	if (target)
	{
		if(Dist < AttackDist)
		{
			return EGhostState::Attack;
		}

		else if (Dist < IdleDist)
		{
			//추격하는 상태로 전환
			return EGhostState::MoveToward;
		}
	}
	return EGhostState::Idle;
}

void State_BlackFigure_Idle::Exit()
{
	AIComp->BlackboardComp->ClearValue(TEXT("State"));
}
