// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Joystick/BackGroundTouchWidget.h"
#include "PlayerCharacter.h"

void UBackGroundTouchWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerChar = Cast<APlayerCharacter>(GetOwningPlayerPawn());

}

//void UBackGroundTouchWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//	if (bIsTouching && PlayerChar)
//	{
//		if (PC)
//		{
//			float Current_X, Current_Y;
//			PC->GetInputMouseDelta(Current_X, Current_Y);
//
//			if (Current_X != 0.f || Current_Y != 0.f)
//			{
//				FVector2D CurrentTouchLocation(Current_X, Current_Y);
//				FVector2D TouchDist = CurrentTouchLocation - TouchPos;
//
//				UE_LOG(LogTemp, Warning, TEXT("Touch Start At: %f, %f"), Current_X, Current_Y);
//				//UE_LOG(LogTemp, Warning, TEXT("Touching! Delta X: %f"), TouchDist.X);
//
//				PlayerChar->AddControllerYawInput(Current_X * RotationSensitivity);
//				PlayerChar->AddControllerPitchInput(Current_Y * RotationSensitivity * -1.0f);
//
//				TouchPos = CurrentTouchLocation;
//			}
//		}
//	}
//}
FReply UBackGroundTouchWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 기존 터치만 유지
    if (RotationPointerIndex != -1) return FReply::Unhandled();

    bIsTouching = true;
    RotationPointerIndex = InMouseEvent.GetPointerIndex(); // 번호 저장

    return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UBackGroundTouchWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 움직이고 있는 손가락인지 확인
    if (bIsTouching && InMouseEvent.GetPointerIndex() == RotationPointerIndex) //-> 왼손은 조이스틱 오른손은 돌릴텐데 오른손만 체크하기 위함
    {
        // 얼마나 이동했는지 
        FVector2D TouchDelta = InMouseEvent.GetCursorDelta();

        if (PlayerChar && !TouchDelta.IsNearlyZero()) // 움직임이 제로수준이 아니라면
        {
            //회전하기
            PlayerChar->AddControllerYawInput(TouchDelta.X * RotationSensitivity);
            PlayerChar->AddControllerPitchInput(TouchDelta.Y * RotationSensitivity * 1.0f);
        }

        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply UBackGroundTouchWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 뗸 손가락이 회전시키던 손가락일 때 리셋하기
    if (InMouseEvent.GetPointerIndex() == RotationPointerIndex)
    {
        bIsTouching = false;
        RotationPointerIndex = -1; // 번호 원상복구
        return FReply::Handled().ReleaseMouseCapture();
    }
    return FReply::Unhandled();
}
