// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Patrol/PatrolPoint.h"
#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"

APatrolPoint::APatrolPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    auto* Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
    Billboard->SetupAttachment(RootComponent);

    LineBatch = CreateDefaultSubobject<ULineBatchComponent>(TEXT("LineBatch"));
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

    for (APatrolPoint* Point : ConnectedPoints)
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