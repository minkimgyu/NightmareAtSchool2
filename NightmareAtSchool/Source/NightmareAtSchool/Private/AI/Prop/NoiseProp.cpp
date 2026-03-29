// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Prop/NoiseProp.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANoiseProp::ANoiseProp()
{
    PrimaryActorTick.bCanEverTick = false; // 소음 발생기는 보통 Tick이 필요 없습니다.

    // 1. 메쉬 컴포넌트 생성 및 루트 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 콜라이더 생성 및 루트 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->SetupAttachment(RootComponent);

    CollisionComponent->SetSphereRadius(100.0f);
    // 콜라이더가 오버랩 이벤트를 발생시키도록 설정
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));

    // 메쉬의 경우 캐릭터와 부딪히지 않게 하려면 NoCollision이나 Overlap만 되게 설정하세요.
    MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ANoiseProp::BeginPlay()
{
	Super::BeginPlay();
	
    // 오버랩 이벤트 바인딩
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANoiseProp::OnOverlapBegin);
}

void ANoiseProp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

    // 디버그 1: 무언가 부딪히긴 했는지 확인
    if (GEngine)
    {
        FString OverlapMsg = FString::Printf(TEXT("Overlap Detected! Actor: %s"), OtherActor ? *OtherActor->GetName() : TEXT("None"));
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, OverlapMsg);
    }


    // 1. 플레이어 태그 확인 (블루프린트에서 사용했던 방식 유지)
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        // 디버그 2: 태그가 있는지 확인 (태그 오타 방지용 로그)
        bool bHasTag = OtherActor->ActorHasTag(FName("Player"));
        if (!bHasTag && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Actor found but 'Player' tag is MISSING!"));
        }

        // 2. AI Perception System에 소음 리포트
        // Instigator(소음 유발자)를 OtherActor(플레이어)로 지정하면 AI가 플레이어 위치를 추적하기 쉬워집니다.
        //UAIPerceptionSystem::OnNoiseEvent(GetWorld(), GetActorLocation(), NoiseLoudness, OtherActor, NoiseRange);

        // OtherActor를 APawn으로 캐스팅하여 전달 (플레이어 캐릭터가 Pawn을 상속받으므로 가능)
        APawn* NoiseMaker = Cast<APawn>(OtherActor);
        MakeNoise(NoiseLoudness, NoiseMaker, GetActorLocation(), NoiseMaxRange, TEXT("PropNoise"));

        // 디버그 메시지 (필요 시 주석 해제하여 확인하세요)
        //if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("MakeNoise Triggered!"));

        // 2. 디버그 서클 그리기
        if (bShowDebugCircle)
        {
            // 소음 발생 지점 (유리조각 위치)
            FVector DebugLocation = GetActorLocation();

            // 소음의 강도나 범위에 비례하여 시각화 (여기서는 50 단위를 기본 크기로 잡았습니다)
            float CircleRadius = 50.0f * NoiseLoudness;

            DrawDebugCircle(
                GetWorld(),
                DebugLocation,
                CircleRadius,
                32,                     // 선 세그먼트
                DebugCircleColor,
                false,                  // 영구 지속 여부
                DebugCircleLifeTime,    // 지속 시간
                0,                      // 우선순위
                DebugThickness,         // 선 두께
                FVector(0, 0, 1),       // 원이 바라보는 방향 (위쪽)
                FVector(1, 0, 0),       // 수평 벡터
                false                   // 채우기 여부
            );
        }
    }
}