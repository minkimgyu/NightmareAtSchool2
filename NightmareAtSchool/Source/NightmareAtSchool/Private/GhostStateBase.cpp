// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GhostStateBase.h"

GhostStateBase::GhostStateBase(ABaseGhostAIController* _AIComp)
{
	AIComp = _AIComp;
	BBComp = AIComp->GetBlackboardComponent();
}

GhostStateBase::~GhostStateBase()
{
}
