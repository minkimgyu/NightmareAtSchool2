// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/PushPullDoor.h"

#include "InteractionInterface.h" // APlayerCharacter.h에서 사용되던 인터페이스
#include "InteractorInterface.h"

#include "Kismet/GameplayStatics.h" // PlaySoundAtLocation 사용을 위해 필요

#include "InteractableObject/DoorBase.h"

#include "PlayerCharacter.h" // Interact 함수에서 사용


// Sets default values
APushPullDoor::APushPullDoor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
    LeftDoorMesh->SetupAttachment(Root);

    RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
    RightDoorMesh->SetupAttachment(Root);

    // Marker는 문이 열릴 때 DoorMesh가 도달해야 할 최종 위치를 지정합니다.
    // Blueprint에서 이 Marker의 위치를 조정하여 문이 열리는 거리를 설정합니다.
}

void APushPullDoor::BeginFocus()
{
    Super::BeginFocus();

    // 메시 아웃라인이나 강조 효과를 처리할 수 있습니다.
    LeftDoorMesh->SetRenderCustomDepth(true);
    RightDoorMesh->SetRenderCustomDepth(true);
}

void APushPullDoor::EndFocus()
{
    // 강조 효과를 제거합니다.
    Super::EndFocus();

    LeftDoorMesh->SetRenderCustomDepth(false);
    RightDoorMesh->SetRenderCustomDepth(false);
}


void APushPullDoor::BeginPlay()
{
    Super::BeginPlay();

    // 문 닫힘 위치 (DoorMesh의 초기 상대 회전)를 저장합니다.
    // 이는 DoorMesh가 RootComponent에 부착된 경우 RootComponent에 대한 상대 회전입니다.
    // 양쪽 문 메시의 초기 상대 회전을 저장합니다.
    ClosedRotationLeft = LeftDoorMesh->GetRelativeRotation();
    ClosedRotationRight = RightDoorMesh->GetRelativeRotation();
}

void APushPullDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 문 상태에 따라 부드러운 회전 처리
    RotateDoor(DeltaTime); // ⬅️ 회전 함수 호출
}

void APushPullDoor::RotateDoor(float DeltaTime)
{
    // 1. 목표 회전 설정
    FRotator TargetRotationL;
    FRotator TargetRotationR;

    if (bIsOpen)
    {
        // 열린 상태:
        // LeftDoorMesh는 OpenAngle만큼 양수(+) Yaw 축 회전
        TargetRotationL = ClosedRotationLeft;
        TargetRotationL.Yaw += OpenAngle;

        // RightDoorMesh는 OpenAngle만큼 음수(-) Yaw 축 회전 (반대 방향)
        TargetRotationR = ClosedRotationRight;
        TargetRotationR.Yaw -= OpenAngle;
    }
    else
    {
        // 닫힌 상태: 초기 회전으로 돌아갑니다.
        TargetRotationL = ClosedRotationLeft;
        TargetRotationR = ClosedRotationRight;
    }

    // 2. 왼쪽 문 회전 보간 및 업데이트
    FRotator CurrentRotationL = LeftDoorMesh->GetRelativeRotation();
    FRotator NewRotationL = FMath::RInterpTo(CurrentRotationL, TargetRotationL, DeltaTime, RotationSpeed);
    LeftDoorMesh->SetRelativeRotation(NewRotationL);

    // 3. 오른쪽 문 회전 보간 및 업데이트
    FRotator CurrentRotationR = RightDoorMesh->GetRelativeRotation();
    FRotator NewRotationR = FMath::RInterpTo(CurrentRotationR, TargetRotationR, DeltaTime, RotationSpeed);
    RightDoorMesh->SetRelativeRotation(NewRotationR);

    // 4. 회전 완료 확인 및 Tick 비활성화
    // 두 문이 모두 목표 회전에 도달했는지 확인합니다.
    if (NewRotationL.Equals(TargetRotationL, 1.0f) && NewRotationR.Equals(TargetRotationR, 1.0f))
    {
        // 최종적으로 목표 회전값으로 설정 (오차 방지)
        //LeftDoorMesh->SetRelativeRotation(TargetRotationL);
        //RightDoorMesh->SetRelativeRotation(TargetRotationR);

        // 문이 멈추면 Tick 비활성화
        SetActorTickEnabled(false);
    }
}

void APushPullDoor::HandleInteraction(IInteractorInterface* Interactor)
{
    if (!Interactor) return;

    Super::HandleInteraction(Interactor);

    if (bIsOpen)
    {
        if (DoorCloseSound)
        {
            // 소리가 캐릭터 위치가 아닌 '문'의 위치에서 나게 하여 입체감을 줍니다.
            FVector SoundLocation = GetActorLocation();
            UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, SoundLocation);
        }

        // 닫는 로직 (C++ 회전 시작)
        bIsOpen = false;
        InteractableData.Action = FText::FromString("Open"); // ⬅️ ActionName으로 수정
    }
    else
    {
        if (DoorOpenSound)
        {
            // 소리가 캐릭터 위치가 아닌 '문'의 위치에서 나게 하여 입체감을 줍니다.
            FVector SoundLocation = GetActorLocation();
            UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, SoundLocation);
        }

        // 여는 로직 (C++ 회전 시작)
        bIsOpen = true;

        // 🚨 이 부분이 중요합니다. 플레이어 위치에 따라 회전 방향(OpenAngle)을 결정합니다.
        if (IsPlayerBehindDoor(Interactor))
        {
            // 플레이어가 문 뒤(당기기)에 있으면 문이 플레이어 쪽으로 열리도록 양수 각도 설정 (예: 90도)
            OpenAngle = FMath::Abs(OpenAngle);
        }
        else
        {
            // 플레이어가 문 앞(밀기)에 있으면 문이 플레이어 반대쪽으로 열리도록 음수 각도 설정 (예: -90도)
            OpenAngle = -FMath::Abs(OpenAngle);
        }

        InteractableData.Action = FText::FromString("Close"); // ⬅️ ActionName으로 수정
    }


    // 회전을 시작하기 위해 Tick을 활성화합니다.
    SetActorTickEnabled(true);

    Interactor->UpdateInteractionWidget(&InteractableData);
}

bool APushPullDoor::IsPlayerBehindDoor(IInteractorInterface* Interactor)
{
    if (!Interactor) return false;

    // 1. 문에서 플레이어로 향하는 벡터
    FVector DoorToPlayer = Interactor->GetInteractorActor()->GetActorLocation() - GetActorLocation();
    DoorToPlayer.Normalize();

    // 2. 문의 정면 방향 벡터 (예: X축)
    FVector DoorForward = GetActorForwardVector();

    // 3. 내적(Dot Product) 계산
    //   - 내적 값이 양수(> 0): 플레이어가 문의 앞면 방향(DoorForward와 같은 방향)에 위치 (밀기/Push)
    //   - 내적 값이 음수(< 0): 플레이어가 문의 뒷면 방향(DoorForward와 반대 방향)에 위치 (당기기/Pull)
    const float DotProduct = FVector::DotProduct(DoorToPlayer, DoorForward);

    // 문의 뒤쪽에 있다면 true 반환 (당겨야 함)
    // 이 문이 어느 쪽으로 열리는지에 따라 기준은 달라질 수 있습니다.
    // 여기서는 DotProduct가 0 미만이면 'Behind'로 간주합니다.
    return DotProduct < 0.0f;
}