// Fill out your copyright notice in the Description page of Project Settings.


#include "AGhost1AIController.h"

#include "AI/Patrol/PatrolManager.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "State_BlackFigure_Idle.h"
#include "State_BlackFigure_MoveToward.h"
#include "State_BlackFigure_Attack.h"

void AAGhost1AIController::OnPossess(APawn* InPawn)
{
	ABaseGhostAIController::OnPossess(InPawn);

	//처음 상태는 대기상태로 설정
	CurrentState = EGhostState::Idle;

	GStateBase = new State_BlackFigure_Idle(this);
	GStateBase->Enter();
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

void AAGhost1AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GStateBase) return;

	EGhostState GetState = GStateBase->InputHandler();
	if (CurrentState != GetState)
	{
		GStateBase->Exit();
		delete GStateBase;
		GStateBase = nullptr;

		switch (GetState)
		{
		case EGhostState::Idle:
			GStateBase = new State_BlackFigure_Idle(this);
			CurrentState = EGhostState::Idle;
			break;

		case EGhostState::MoveToward:
			GStateBase = new State_BlackFigure_MoveToward(this);
			CurrentState = EGhostState::MoveToward;
			break;

		case EGhostState::Attack:
			GStateBase = new State_BlackFigure_Attack(this);
			CurrentState = EGhostState::Attack;
			break;

		case EGhostState::RunAway:
			//GStateBase = new State_BlackFigure_RunAway(this);
			CurrentState = EGhostState::RunAway;
			break;
		}
		if (GStateBase)
		{
			GStateBase->Enter();
		}
	}
}