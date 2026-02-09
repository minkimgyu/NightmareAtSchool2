// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "State_BlackFigure.h"

State_BlackFigure::State_BlackFigure(ABaseGhostAIController* _AIComp)
	:GhostStateBase(_AIComp)
{

}

State_BlackFigure::~State_BlackFigure()
{
}
