// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SprintBar.generated.h"

/**
 * 
 */

class UProgressBar; // 프로그레스 바 포워드 선언
class APlayerCharacter;

UCLASS()
class NIGHTMAREATSCHOOL_API USprintBar : public UUserWidget
{
	GENERATED_BODY()

public:
	/** UI 업데이트를 위해 Tick을 사용합니다. */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	/**
	 * 위젯에 배치된 프로그레스 바 위젯에 대한 바인딩입니다.
	 * 반드시 블루프린트 위젯에서 이 변수 이름과 일치하는 'UProgressBar'를 'Is Variable'로 설정해야 합니다.
	 */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* LeftSprintBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* RightSprintBar;

private:
	/** 플레이어 캐릭터 캐싱을 위한 변수 */
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	/** 플레이어 캐릭터를 캐싱하여 반환하는 함수 */
	APlayerCharacter* GetPlayerCharacter();

	/** 스프린트 바의 퍼센트 값을 업데이트하는 핵심 로직 */
	void UpdateSprintBar(float DeltaTime);
};
