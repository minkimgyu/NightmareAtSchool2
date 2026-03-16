// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghost1ViewAngleComponent.h"
#include "Ghost1.h"
#include "BlackFigureDataAsset.h"

void UGhost1ViewAngleComponent::BeginPlay()
{
	UMonsterViewAngleComponent::BeginPlay();

	AGhost1* Ghost = Cast<AGhost1>(GetOwner());
	if (!Ghost) return; // 고스트자체가 없으면 return
	if (!Ghost->BlackFigureData) return; // 아직 데이터가 설정전이라면 return

	ViewDistance = Ghost->BlackFigureData->MoveTowardDistance;
	ViewAngle = Ghost->BlackFigureData->ViewAngle;
}