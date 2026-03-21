// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/NavBlockOnlyActor.h"

// ✅ 반드시 필요
#include "Components/BoxComponent.h"
#include "NavModifierComponent.h"
// ✅ 수정 후
#include "NavAreas/NavArea_Null.h"

// Sets default values
ANavBlockOnlyActor::ANavBlockOnlyActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 (Box를 루트로 사용)
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    // 🔥 UE5 권장 방식
    SetRootComponent(Box);

    // 🔴 핵심: 모든 충돌 제거 (플레이어, AI, 레이캐스트 다 통과)
    Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // NavModifier 생성
    NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));

    // 🔴 핵심: 해당 영역을 NavMesh에서 제거
    NavModifier->SetAreaClass(UNavArea_Null::StaticClass());
}

// Called when the game starts or when spawned
void ANavBlockOnlyActor::BeginPlay()
{
	Super::BeginPlay();
	
}