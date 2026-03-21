// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/HP/PlayerHP.h"
#include "Components/TextBlock.h" // 텍스트 블록 헤더 추가

void UPlayerHP::UpdateHPText(float CurrentHP)
{
    if (HPText)
    {
        // 음수 체력 방지 (0 미만은 0으로 표시)
        float DisplayHP = FMath::Max(CurrentHP, 0.0f);

        // 문자열 포맷팅: "HP : 80 / 100"
        FString HPString = FString::Printf(TEXT("%d"), FMath::FloorToInt(DisplayHP));

        HPText->SetText(FText::FromString(HPString));

        OnHPChanged.Broadcast(CurrentHP);
    }
}