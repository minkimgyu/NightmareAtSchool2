// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Patrol/PatrolPoint.h"

#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"

APatrolPoint::APatrolPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    auto* Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard")); // 스폰 위치설정? 하는거 같은데
    Billboard->SetupAttachment(RootComponent);

    LineBatch = CreateDefaultSubobject<ULineBatchComponent>(TEXT("LineBatch")); // 경로 시각화용 컴포넌트
    LineBatch->SetupAttachment(RootComponent);
    LineBatch->bCalculateAccurateBounds = true;
}

void APatrolPoint::RefreshConnectionLines()
{
//#if WITH_EDITOR
    if (!LineBatch) return;

    LineBatch->Flush();

    if (!bShowConnections)
        return;

    for (APatrolPoint* Point : ConnectedPoints) // 블루프린트 에디터에서 받아온 모든 연결 점들을 보여주기용
    {
        if (!Point) continue;

        LineBatch->DrawLine(
            GetActorLocation(),
            Point->GetActorLocation(),
            FLinearColor::Red,
            SDPG_World,
            10.f,
            0.f
        );
    }
//#endif
}