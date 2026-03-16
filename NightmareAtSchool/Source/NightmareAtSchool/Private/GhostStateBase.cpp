// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostStateBase.h"
#include "BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

GhostStateBase::GhostStateBase(ABaseGhostAIController* _AIComp)
{
	AIComp = _AIComp;
	BBComp = AIComp->GetBlackboardComponent();
}

GhostStateBase::~GhostStateBase()
{
}
