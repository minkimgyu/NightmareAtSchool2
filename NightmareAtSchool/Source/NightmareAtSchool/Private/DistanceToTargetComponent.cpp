// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceToTargetComponent.h"
//#include "AIController.h"
#include "MonsterViewAngleComponent.h"
//#include "BehaviorTree/BlackboardComponent.h"

// Sets default values for this component's properties
UDistanceToTargetComponent::UDistanceToTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDistanceToTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	MonsterViewAngleEvent();
	//GetAIController();
	// ...
	
}


// Called every frame
void UDistanceToTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Target)
	{
		TargetAndGhostDist = GetDistanceToTarget();
		//OnTargetDist.Broadcast(TargetAndGhostDist);
		UE_LOG(LogTemp, Warning, TEXT("Player is exist"));
	}

	// ...
}

//1. 시야각 이벤트
void UDistanceToTargetComponent::MonsterViewAngleEvent()
{
	MonsterViewAngleComp = GetOwner()->FindComponentByClass<UMonsterViewAngleComponent>();

	if (MonsterViewAngleComp)
	{
		MonsterViewAngleComp->OnTarget.AddDynamic(this, &UDistanceToTargetComponent::SetTarget);
	}
}

void UDistanceToTargetComponent::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;  // nullptr도 가능
}

float UDistanceToTargetComponent::GetDistanceToTarget() const
{
	if (!Target) return 0.f;
	return FVector::Distance(GetOwner()->GetActorLocation(), Target->GetActorLocation());
}
////
//void UDistanceToTargetComponent::SaveBlackboard()
//{
//	if (AIController)
//	{
//		Blackboard = AIController->GetBlackboardComponent();
//
//		if (Target)
//		{
//			if (Blackboard)
//			{
//				UE_LOG(LogTemp, Warning, TEXT("SUCCESS BLACKBOARD"));
//				Blackboard->SetValueAsFloat(TEXT("TargetDistance"), GetDistanceToTarget());
//			}
//		}
//	}
//}
////
//void UDistanceToTargetComponent::GetAIController()
//{
//	UE_LOG(LogTemp, Warning, TEXT("GetAIController"));
//	OwnerPawn = Cast<APawn>(GetOwner());
//	if (!OwnerPawn)
//	{
//		return;
//	}
//
//	AIController = Cast<AAIController>(OwnerPawn->GetController());
//	if (!AIController)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("NOT DISCOVER AICONTROLLER"));
//		return;
//	}
//}

