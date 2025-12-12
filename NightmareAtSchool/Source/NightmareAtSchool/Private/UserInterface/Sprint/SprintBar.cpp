// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Sprint/SprintBar.h"

#include "Components/ProgressBar.h" // UProgressBar 사용을 위해 필요
#include "PlayerCharacter.h"       // APlayerCharacter 정보 접근을 위해 필요
#include "Kismet/GameplayStatics.h" // GetPlayerPawn 사용을 위해 필요

void USprintBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 매 프레임 스프린트 바 업데이트
	UpdateSprintBar(InDeltaTime);
}

APlayerCharacter* USprintBar::GetPlayerCharacter()
{
	// 캐릭터가 아직 캐싱되지 않았거나 유효하지 않다면 새로 가져옵니다.
	if (!PlayerCharacter)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	}
	return PlayerCharacter;
}

void USprintBar::UpdateSprintBar(float DeltaTime)
{
	if (!LeftSprintBar || !RightSprintBar)
	{
		// 프로그레스 바가 유효하지 않으면 함수를 종료합니다.
		return;
	}

	// APlayerCharacter로부터 스프린트 정보를 가져옵니다.
	if (const APlayerCharacter* Player = GetPlayerCharacter())
	{
		const float CurrentDuration = Player->GetCurrentSprintDuration();
		const float MaxDuration = Player->GetMaxSprintDuration();

		if (MaxDuration > 0.0f)
		{
			// 현재 남은 스프린트 시간의 비율 (0.0 ~ 1.0)
			const float CurrentPercent = CurrentDuration / MaxDuration;

			//// 실제 구현에서는 Right Bar의 Fill Type을 "Right to Left"로 설정해야 합니다.
			//// 여기서는 LeftPercent와 RightPercent가 (0~1) 범위에서 대칭적으로 움직이도록 설정합니다.

			LeftSprintBar->SetPercent(CurrentPercent);
			RightSprintBar->SetPercent(CurrentPercent);
		}
	}
}