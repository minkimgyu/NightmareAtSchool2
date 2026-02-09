// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GhostStateBase.h"
#include "CoreMinimal.h"
//
/**
 * 
 */
class NIGHTMAREATSCHOOL_API State_BlackFigure : public GhostStateBase
{
public:
	State_BlackFigure(ABaseGhostAIController* _AIComp);
	virtual ~State_BlackFigure();
};
