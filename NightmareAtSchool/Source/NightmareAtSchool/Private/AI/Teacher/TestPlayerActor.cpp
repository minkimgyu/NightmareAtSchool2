// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/TestPlayerActor.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATestPlayerActor::ATestPlayerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	Capsule->InitCapsuleSize(34.f, 88.f);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	StimuliSource =
		CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->RegisterForSense(UAISense_Hearing::StaticClass());
	//StimuliSource->bAutoRegisterAsSource = true;

	// 명확하게 Player 태그 부여
	Tags.AddUnique(TEXT("Player"));
}

// Called when the game starts or when spawned
void ATestPlayerActor::BeginPlay()
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