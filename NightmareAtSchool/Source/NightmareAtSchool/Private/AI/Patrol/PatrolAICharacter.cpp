// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Patrol/PatrolAIController.h"          // ★ 반드시 필요
#include "GameFramework/Character.h"         // 보통 포함되어 있음
#include "AI/Patrol/PatrolAICharacter.h"

// Sets default values
APatrolAICharacter::APatrolAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
