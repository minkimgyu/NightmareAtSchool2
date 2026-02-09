// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/TestPlayer.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATestPlayer::ATestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StimuliSource =
		CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->RegisterForSense(UAISense_Hearing::StaticClass());
	//StimuliSource->bAutoRegisterAsSource = true;

	// 명확하게 Player 태그 부여
	Tags.AddUnique(TEXT("Player"));
}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (StimuliSource)
	{
		StimuliSource->RegisterWithPerceptionSystem();
	}

	if (!Tags.Contains(TEXT("Player")))
	{
		Tags.Add(TEXT("Player"));
	}
}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

