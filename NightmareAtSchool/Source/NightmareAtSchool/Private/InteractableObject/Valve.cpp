// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/Valve.h"

#include "InteractionInterface.h" // APlayerCharacter.h에서 사용되던 인터페이스

#include "Components/StaticMeshComponent.h"

#include "InteractorInterface.h"

#include "Kismet/GameplayStatics.h" // PlaySoundAtLocation 사용을 위해 필요


// Sets default values
AValve::AValve()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 컴포넌트 설정
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    ValveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ValveMesh"));
    ValveMesh->SetupAttachment(Root);

    // 기본 상태 초기화
    bIsLock = false;

    UE_LOG(LogTemp, Log, TEXT("Name1: %s Desc1: %s"),
        *InteractableData.Name.ToString(),
        *InteractableData.Action.ToString());
}

// Called when the game starts or when spawned
void AValve::BeginPlay()
{
	Super::BeginPlay();
	
    // 상호작용 데이터 초기화
    InteractableData.InteractionDuration = 0.0f; // 즉시 상호작용
    InteractableData.Action = FText::FromString("Lock");
    InteractableData.Name = FText::FromString("the Valve");

    UE_LOG(LogTemp, Log, TEXT("Name: %s Desc: %s"),
        *InteractableData.Name.ToString(),
        *InteractableData.Action.ToString());

    ClosedRotation = ValveMesh->GetRelativeRotation();
}

// Called every frame
void AValve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 문 상태에 따라 부드러운 회전 처리
    RotateValve(DeltaTime); // ⬅️ 회전 함수 호출
}

void AValve::Interact(IInteractorInterface* Interactor)
{
    // 문 상호작용을 처리하는 핵심 로직 호출
    HandleInteraction(Interactor);
}

void AValve::BeginFocus()
{
    // 메시 아웃라인이나 강조 효과를 처리할 수 있습니다.
    ValveMesh->SetRenderCustomDepth(true);
}

void AValve::EndFocus()
{
    // 강조 효과를 제거합니다.
    ValveMesh->SetRenderCustomDepth(false);
}


void AValve::RotateValve(float DeltaTime)
{
    // 1. 목표 회전 설정
    FRotator TargetRotation;

    if (bIsLock)
    {
        // 열린 상태:
        // LeftDoorMesh는 OpenAngle만큼 양수(+) Yaw 축 회전
        TargetRotation = ClosedRotation;
        TargetRotation.Roll += OpenAngle;
    }
    else
    {
        // 닫힌 상태: 초기 회전으로 돌아갑니다.
        TargetRotation = ClosedRotation;
    }

    // 2. 왼쪽 문 회전 보간 및 업데이트
    FRotator CurrentRotation = ValveMesh->GetRelativeRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
    ValveMesh->SetRelativeRotation(NewRotation);

    // 4. 회전 완료 확인 및 Tick 비활성화
    // 두 문이 모두 목표 회전에 도달했는지 확인합니다.
    if (NewRotation.Equals(TargetRotation, 1.0f))
    {
        // 문이 멈추면 Tick 비활성화
        SetActorTickEnabled(false);
    }
}

// ADoorBase의 HandleInteraction 기본 구현 (빈 함수 또는 간단한 로직)
void AValve::HandleInteraction(IInteractorInterface* Interactor)
{
    //Super::HandleInteraction(PlayerCharacter);

    if (!Interactor) return;
    if (bIsLock) return;

    if (bIsLock == false)
    {
        // 닫는 로직 (C++ 회전 시작)
        bIsLock = true;

        // 등록된 모든 리스너에게 이벤트 전송 (블루프린트의 Call)
        OnValveOpened.Broadcast();

        if (ValveLockSound)
        {
            FVector SoundLocation = GetActorLocation();
            UGameplayStatics::PlaySoundAtLocation(this, ValveLockSound, SoundLocation);
        }
    }

    // 로그를 찍어서 함수가 실행되는지 확인하세요!
    UE_LOG(LogTemp, Warning, TEXT("Valve Interacted! bIsLock: %s"), bIsLock ? TEXT("True") : TEXT("False"));

    // 회전을 시작하기 위해 Tick을 활성화합니다.
    SetActorTickEnabled(true);
    Interactor->UpdateInteractionWidget(&InteractableData);
}