// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoystickWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UJoystickWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitJoystick(FVector2D NativeCenter);

	UFUNCTION(BlueprintCallable)
	void UpdateJoystick(FVector2D TouchLocation);

	UFUNCTION(BlueprintCallable)
	void ResetJoystick();

	UFUNCTION(BlueprintPure)
	FVector2D GetMovementInput() const { return MovementInput; };

protected:

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(meta = (Bindwidget), BlueprintReadOnly)
	class UImage* IM_JoystickBackground;

	UPROPERTY(meta = (Bindwidget))
	class UImage* IM_JoystickMain;

	FVector2D JoystickCenter;
	FVector2D MovementInput;
	bool bIsTouching = false;

	UPROPERTY(EditAnywhere, Category = "Joystick Settings")
	float MaxRadius = 70.0f;

private:
	int32 JoystickPointerIndex = -1;
};
