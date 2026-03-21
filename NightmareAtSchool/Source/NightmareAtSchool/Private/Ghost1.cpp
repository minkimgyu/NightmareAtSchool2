// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost1.h"
#include "Components/BoxComponent.h"
#include "BlackFigureDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGhost1::AGhost1()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
    //루트에 그냥 갔다 박아버리기
    AttackCollisionBox->SetupAttachment(RootComponent);

    //앞에다가 설정해놓음
    AttackCollisionBox->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
    AttackCollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f)); // 콜리전 박스 크기

    AttackCollisionBox->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void AGhost1::BeginPlay()
{
    Super::BeginPlay();

    AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGhost1::OnAttackOverlap);
}

// Called every frame
void AGhost1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AGhost1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGhost1::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        // 충돌한 적이 있는 확인하고
        if (HitActors.Contains(OtherActor))
            return;

        // 데미지 적용
        UGameplayStatics::ApplyDamage(OtherActor, 20.0f, GetController(), this, nullptr);

        // 충돌했으면 중복충돌 방지로 넣어놓기
        HitActors.Add(OtherActor);

        UE_LOG(LogTemp, Warning, TEXT("%s 타격 성공!"), *OtherActor->GetName());
    }
}

void AGhost1::OnAttackStart()
{
    // 리스트 초기화
    HitActors.Empty();

    // 공격 모션에 맞춰서 활성화
    AttackCollisionBox->SetGenerateOverlapEvents(true);
}

void AGhost1::OnAttackEnd()
{
    // 공격 모션 종료에 맞춰서 비활성화
    AttackCollisionBox->SetGenerateOverlapEvents(false);
}
