// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Patrol/PatrolAIController.h"

#include "AI/Patrol/PatrolManager.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

APatrolAIController::APatrolAIController()
{
    // C++에서 컴포넌트를 생성하여 할당합니다.
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")); // 언리얼 시야각컴포넌트를 하위에 추가한다.
    SetPerceptionComponent(*PerceptionComponent);

    // 기존 HearingConfig 아래에 추가
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

    // BeginPlay에서는 "값만" 설정합니다.
    if (SightConfig)
    {
        SightConfig->SightRadius = SightRadius;
        SightConfig->LoseSightRadius = LoseSightRadius;
        SightConfig->PeripheralVisionAngleDegrees = SightAngle / 2;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    }

    if (HearingConfig)
    {
        HearingConfig->HearingRange = HearingRange;
        HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
        HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
        HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    }

    // BeginPlay에서 준비한 값을 여기서 "적용"
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->ConfigureSense(*HearingConfig);
    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void APatrolAIController::BeginPlay()
{
    Super::BeginPlay();

    // 1. 객체 유효성 검사 로그
    if (SightConfig)
    {
        UE_LOG(LogTemp, Warning, TEXT("SightConfig is Valid. Radius: %f, Angle: %f"),
            SightConfig->SightRadius, SightConfig->PeripheralVisionAngleDegrees);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SightConfig is NULL at BeginPlay!"));
    }

    if (HearingConfig)
    {
        UE_LOG(LogTemp, Warning, TEXT("HearingConfig is Valid. Range: %f"), HearingConfig->HearingRange);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HearingConfig is NULL at BeginPlay!"));
    }

    if (!PatrolBehaviorTree)
    {
        UE_LOG(LogTemp, Error, TEXT("PatrolBehaviorTree is NULL"));
        return;
    }
    
    // 1. Perception 컴포넌트가 있는지 확인하고 이벤트 연결
    UE_LOG(LogTemp, Warning, TEXT("Perception Connected!"));
    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &APatrolAIController::OnPerceptionUpdated);

    const bool bBTStarted = RunBehaviorTree(PatrolBehaviorTree);

    UE_LOG(LogTemp, Error, TEXT("RunBehaviorTree result = %s"),
        bBTStarted ? TEXT("SUCCESS") : TEXT("FAIL"));

    if (bBTStarted)
    {
        // 순찰 모드 속도 지정
        UBlackboardComponent* BB = GetBlackboardComponent();
        if (BB)
        {
            BB->SetValueAsFloat(TEXT("MovementSpeed"), PatrolSpeed); // 예: 300.f
        }
    }
}

void APatrolAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Error, TEXT("Controller class: %s"), *GetClass()->GetName());

    // 월드에 배치된 PatrolManager 찾기
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        ABasePatrolManager::StaticClass(),
        Found
    );

    if (Found.Num() > 0)
    {
        PatrolManager = Cast<ABasePatrolManager>(Found[0]);
    }
}

void APatrolAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Error, TEXT("OnUpdated - Sound"));

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB || !Actor) return;

    // 1. 시각 센서인지 확인
    if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Spotted!"));

        // 2. 감지된 액터가 "Player" 태그를 가졌는지 확인
        if (Stimulus.WasSuccessfullySensed() && Actor->ActorHasTag(TEXT("Player")))
        {
            // 플레이어 발견: 추적 시작
            BB->SetValueAsObject(TEXT("TargetActor"), Actor);
            BB->SetValueAsBool(TEXT("IsChasing"), true);
            BB->SetValueAsFloat(TEXT("MovementSpeed"), ChasingSpeed); // 전력 질주 속도

            // 추적 중에는 사운드 조사(Investigating) 중단
            BB->SetValueAsBool(TEXT("IsInvestigating"), false);

            UE_LOG(LogTemp, Warning, TEXT("Player Spotted! Start Chasing."));
        }
        // 플레이어를 놓친 경우
        else if (!Stimulus.WasSuccessfullySensed())
        {
            // 1. 추적 상태 해제
            BB->SetValueAsBool(TEXT("IsChasing"), false);

            // 2. 속도를 조사 속도로 변경 (추적이 종료되었으므로)
            BB->SetValueAsFloat(TEXT("MovementSpeed"), InvestigateSpeed);

            // 3. 플레이어의 마지막 위치를 조사 위치로 설정
            BB->SetValueAsVector(TEXT("TargetLocation"), Actor->GetActorLocation());

            // 4. 조사 모드 활성화 (추적하던 곳을 가서 확인하게 함)
            BB->ClearValue(TEXT("IsInvestigating")); // 변화를 인지시키기 위해 초기화 후 설정
            BB->SetValueAsBool(TEXT("IsInvestigating"), true);

            UE_LOG(LogTemp, Warning, TEXT("Player Lost! Switching to Investigation speed."));
        }
    }
    // 이제 Actor와 Stimulus를 직접 사용할 수 있습니다.
    else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>() && Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Error, TEXT("OnUpdated - Sound          2"));

        // 1. 위치 갱신
        BB->SetValueAsVector(TEXT("TargetLocation"), Stimulus.StimulusLocation);

        // 조사 속도로 블랙보드 값 변경
        BB->SetValueAsFloat(TEXT("MovementSpeed"), InvestigateSpeed);

        // 2. 핵심: 데코레이터가 '변화'를 확실히 인지하도록 이벤트를 강제로 발생시킵니다.
            // 이미 true인 경우 변화로 인식되지 않을 수 있으므로, 잠시 Clear한 뒤 다시 세팅합니다.
        BB->ClearValue(TEXT("IsInvestigating"));
        BB->SetValueAsBool(TEXT("IsInvestigating"), true);

        UE_LOG(LogTemp, Warning, TEXT("New Sound Detected! Updating Location."));
    }
}