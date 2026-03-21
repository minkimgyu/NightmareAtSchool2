// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Patrol/PatrolAICharacter.h"
#include "Components/InteractionComponent.h" // 인클루드 추가
#include "GameFramework/Character.h"         // 보통 포함되어 있음

// Sets default values
APatrolAICharacter::APatrolAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 상호작용 컴포넌트 생성 및 기본 설정
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionSight = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionSight"));
	InteractionSight->SetupAttachment(RootComponent);

	// 컴포넌트의 델리게이트와 HUD 업데이트 로직 바인딩
	if (InteractionComp)
	{
		InteractionComp->InitializeInteraction(InteractionSight);
	}

	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AIControllerClass = APatrolAIController::StaticClass();
}
//
//// Called when the game starts or when spawned
//void APatrolAICharacter::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void APatrolAICharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void APatrolAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}
//
