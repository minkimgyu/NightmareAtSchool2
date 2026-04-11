// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackGroundTouchWidget.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UBackGroundTouchWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	APlayerCharacter* PlayerChar;
	
protected:

	virtual void NativeConstruct() override;

	//그냥 실행안되길래 포기한 함수
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//모바일 전용 새롭게 등장하신 함수들
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	bool bIsTouching = false;

	//FVector2D TouchPos; // 더미 터치 좌표

	//감도 조절용
	UPROPERTY(EditAnywhere, Category = "Input")
	float RotationSensitivity = 0.5f;
	
private:
	int32 RotationPointerIndex = -1; // 시야 회전을 담당할 손가락 번호
};
