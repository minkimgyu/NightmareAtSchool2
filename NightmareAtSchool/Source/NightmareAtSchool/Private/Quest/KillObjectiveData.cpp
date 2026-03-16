// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/KillObjectiveData.h"

void UKillObjectiveData::OnEvent(FName TargetEvent, int32 Amount)
{
    if (TargetEvent == MonsterID)
    {
        //CurrentAmount += Amount;
    }
}