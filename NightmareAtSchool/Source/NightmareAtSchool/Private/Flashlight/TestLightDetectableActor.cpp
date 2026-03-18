// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight/TestLightDetectableActor.h"

// Sets default values
ATestLightDetectableActor::ATestLightDetectableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATestLightDetectableActor::SetIlluminated(bool bState)
{
    if (bIsIlluminated != bState)
    {
        bIsIlluminated = bState;

        // 테스트용 로그 출력
        if (bIsIlluminated)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s가 손전등 빛에 노출되었습니다!"), *GetName());
        }
    }
}