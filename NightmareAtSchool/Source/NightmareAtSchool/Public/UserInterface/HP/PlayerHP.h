// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerHP.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UPlayerHP : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 블루프린트 내 Image의 이름을 'HPPersonaIcon'으로 설정해야 합니다.
    UPROPERTY(meta = (BindWidget))
    UImage* HPPersonaIcon;

    // 블루프린트 내 TextBlock의 이름을 'HPText'로 설정해야 합니다.
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HPText;

    // 현재 HP와 최대 HP를 받아 "100 / 100" 형태로 출력
    void UpdateHPText(float CurrentHP);
};
