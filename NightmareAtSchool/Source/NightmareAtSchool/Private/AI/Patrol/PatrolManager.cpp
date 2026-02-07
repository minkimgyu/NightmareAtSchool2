// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Patrol/PatrolManager.h"

#include "AI/Patrol/PatrolPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APatrolManager::APatrolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void APatrolManager::BuildPatrolConnections()
{
    if (!bAutoGenerateConnections) return;

    CollectPatrolPoints();
    GenerateConnections();

    UE_LOG(LogTemp, Log, TEXT("Patrol connections rebuilt (%d points)"),
        AllPatrolPoints.Num());

    RefreshAllConnectionLines();

    UE_LOG(LogTemp, Log, TEXT("Patrol connections rebuilt and lines refreshed"));
}

void APatrolManager::RebuildPatrolConnections()
{
    BuildPatrolConnections();
}

void APatrolManager::CollectPatrolPoints()
{
    AllPatrolPoints.Empty();

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        APatrolPoint::StaticClass(),
        Found
    );

    for (AActor* A : Found)
    {
        if (APatrolPoint* P = Cast<APatrolPoint>(A))
        {
            AllPatrolPoints.Add(P);
        }
    }
}

// Called when the game starts or when spawned
void APatrolManager::BeginPlay()
{
    Super::BeginPlay();
    BuildPatrolConnections();
}

APatrolPoint* APatrolManager::GetRandomStartPoint()
{
    if (AllPatrolPoints.Num() == 0) return nullptr;

    return AllPatrolPoints[FMath::RandRange(0, AllPatrolPoints.Num() - 1)];
}

APatrolPoint* APatrolManager::GetNextPatrolPoint(APatrolPoint* CurrentPoint)
{
    if (!CurrentPoint || CurrentPoint->ConnectedPoints.Num() == 0)
        return nullptr;

    float TotalWeight = 0.f;
    for (APatrolPoint* P : CurrentPoint->ConnectedPoints)
    {
        TotalWeight += P->PreferenceWeight;
    }

    float Rand = FMath::FRandRange(0.f, TotalWeight);

    float Acc = 0.f;
    for (APatrolPoint* P : CurrentPoint->ConnectedPoints)
    {
        Acc += P->PreferenceWeight;
        if (Rand <= Acc)
        {
            return P;
        }
    }

    return nullptr;
}

void APatrolManager::GenerateConnections()
{
    if (AllPatrolPoints.Num() < 2)
        return;

    // 기존 연결 제거
    for (APatrolPoint* P : AllPatrolPoints)
    {
        P->ConnectedPoints.Empty();
    }

    TSet<APatrolPoint*> Visited;
    Visited.Add(AllPatrolPoints[0]);

    while (Visited.Num() < AllPatrolPoints.Num())
    {
        float BestDistSq = TNumericLimits<float>::Max();
        APatrolPoint* From = nullptr;
        APatrolPoint* To = nullptr;

        for (APatrolPoint* V : Visited)
        {
            for (APatrolPoint* Candidate : AllPatrolPoints)
            {
                if (Visited.Contains(Candidate))
                    continue;

                float DistSq = FVector::DistSquared(
                    V->GetActorLocation(),
                    Candidate->GetActorLocation()
                );

                if (DistSq < BestDistSq)
                {
                    BestDistSq = DistSq;
                    From = V;
                    To = Candidate;
                }
            }
        }

        if (!From || !To)
            break;

        From->ConnectedPoints.Add(To);
        if (bBidirectional)
        {
            To->ConnectedPoints.Add(From);
        }

        Visited.Add(To);
    }
}

void APatrolManager::RefreshAllConnectionLines()
{
    for (APatrolPoint* Point : AllPatrolPoints)
    {
        if (Point)
        {
            Point->RefreshConnectionLines();
        }
    }
}