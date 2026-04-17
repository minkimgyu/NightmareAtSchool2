// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoystickWidget.generated.h"

/**
 * 
 */

 // 1. 델리게이트 선언 (매개변수로 현재 터치 여부 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoystickTouchChanged, bool, bIsTouching);

UCLASS()
class NIGHTMAREATSCHOOL_API UJoystickWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 2. 블루프린트에서 할당 가능한 델리게이트 변수
	UPROPERTY(BlueprintAssignable, Category = "Joystick | Events")
	FOnJoystickTouchChanged OnJoystickTouchChanged;

	UFUNCTION(BlueprintCallable)
	void InitJoystick(FVector2D NativeCenter);

	UFUNCTION(BlueprintCallable)
	void UpdateJoystick(FVector2D TouchLocation);

	UFUNCTION(BlueprintCallable)
	void ResetJoystick();

	UFUNCTION(BlueprintPure)
	FVector2D GetMovementInput() const { return MovementInput; };

protected:
	UPROPERTY(meta = (Bindwidget), BlueprintReadOnly)
	class UImage* IM_JoystickBackground;

	UPROPERTY(meta = (Bindwidget))
	class UImage* IM_JoystickMain;

	FVector2D JoystickCenter;
	FVector2D MovementInput;
	bool bIsTouching = false;

	UPROPERTY(EditAnywhere, Category = "Joystick Settings")
	float MaxRadius = 70.0f;

};
