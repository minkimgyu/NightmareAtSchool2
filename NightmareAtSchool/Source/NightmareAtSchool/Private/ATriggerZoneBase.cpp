// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "ATriggerZoneBase.h"

// Sets default values
AATriggerZoneBase::AATriggerZoneBase()
{
    IsCol = false;
	TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComp"));
	RootComponent = TriggerComp;

    TriggerComp->SetCollisionProfileName(TEXT("Trigger"));
	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AATriggerZoneBase::OnTriggerOverlap);
}

void AATriggerZoneBase::OnTriggerOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("FDJISAJFDIOSAJFOIEJWOIAJRLFEWKA"));
    if (IsCol) return; // 이미 한번이라도 반을 방문했다면 더 이상 작동 x
    if (!OtherActor) return; 
    if (!TargetActor) return;
    if (!OtherActor-> ActorHasTag("Player")) return;

    IsCol = true;
    OnTriggered();
}
