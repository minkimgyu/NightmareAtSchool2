// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Joystick/JoystickWidget.h"
#include "Components/Image.h"

void UJoystickWidget::InitJoystick(FVector2D NativeCenter)
{
	JoystickCenter = NativeCenter;
	//UE_LOG(LogTemp, Warning, TEXT("JoystickCenter: X=%f, Y=%f"), JoystickCenter.X, JoystickCenter.Y);
	bIsTouching = true; // 터치했음

	if (IM_JoystickMain)
		IM_JoystickMain->SetOpacity(0.7f); // 처음에는 약간 투명하게 시작
}

void UJoystickWidget::UpdateJoystick(FVector2D TouchLocation)
{
	if (!bIsTouching)
		return;
	//UE_LOG(LogTemp, Warning, TEXT("Center: %s | Touch: %s"), *JoystickCenter.ToString(), *TouchLocation.ToString());
	
	//조이스틱이 Background를 벗어나지 못하게 하는 로직구현
	 
	// 1. 중심점에서 현재 터치좌표까지의 벡터 계산
	FVector2D Offset = TouchLocation - JoystickCenter;

	//UE_LOG(LogTemp, Warning, TEXT("Joystick TouchLocation: X=%f, Y=%f"), TouchLocation.X, TouchLocation.Y);
	//UE_LOG(LogTemp, Warning, TEXT(" Offset: X=%f, Y=%f"), Offset.X, Offset.Y);

	// 2. 거리 제한 (MaxRadius를 기준으로 넘지않게)
	float Distance = Offset.Size();

	//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);

	if (Distance > MaxRadius) // 길이가 정해놓은 값보다 더 크다면
	{
		Offset = Offset.GetSafeNormal() * MaxRadius; // Offset에서 방향벡터구하고 거기에 내가 설정한 최대 반지름곱으로 수정(백그라운드 못 벗어나게하기 위함)
	}

	// 3. 조이스틱 이미지 위치 업데이트 및 투명도 조절
	if (IM_JoystickMain)
	{
		IM_JoystickMain->SetOpacity(1.0f); // 그냥 제일 잘보이는걸로 해버리기
		IM_JoystickMain->SetRenderTranslation(Offset);
	}

	// 4. 움직이는 입력값 정규화시키기 (-1.0 ~ 1.0)범위로

	MovementInput.X = Offset.X / MaxRadius;
	MovementInput.Y = (Offset.Y / MaxRadius) * -1.0f; // 위젯은 아래가 +니까 -1.0 곱해주자
}

void UJoystickWidget::ResetJoystick()
{
	MovementInput = FVector2D::ZeroVector; // 움직임 제로로
	if (IM_JoystickMain)
	{
		IM_JoystickMain->SetOpacity(0.7f); // 투명도 다시 복구
		IM_JoystickMain->SetRenderTranslation(FVector2D::ZeroVector); // 원래 자리로 되돌려 놓기
	}
	bIsTouching = false; // 터치해제
}

FReply UJoystickWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 조이스틱을 처음 눌렀을 때의 좌표를 중심으로 설정
	FVector2D StartLocation = InMouseEvent.GetScreenSpacePosition();

	InitJoystick(StartLocation);

	//조이스틱 조종하는 손가락 저장하기(아마 왼손)
	JoystickPointerIndex = InMouseEvent.GetPointerIndex();

	//터치이동 허용(드래그 허용)
	return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UJoystickWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 조이스틱 조종 중인 손가락일 때만
	if (bIsTouching && InMouseEvent.GetPointerIndex() == JoystickPointerIndex)
	{
		// 현재 드래그중인 위치 가져오기
		FVector2D CurrentLocation = InMouseEvent.GetScreenSpacePosition();

		//조이스틱 업데이트
		UpdateJoystick(CurrentLocation);

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UJoystickWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetPointerIndex() == JoystickPointerIndex)
	{
		ResetJoystick();
		JoystickPointerIndex = -1;

		// 터치이동 해제
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

