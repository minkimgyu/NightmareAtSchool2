// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "InteractorInterface.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 자동 체크가 켜져 있을 때만 라인트레이스 실행 (플레이어용)
	if (bEnableAutoCheck && GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void UInteractionComponent::InitializeInteraction(USceneComponent* InTraceAnchor)
{
	TraceAnchorComponent = InTraceAnchor;
}

void UInteractionComponent::PerformInteractionCheck()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	// 소유자가 Pawn인지 확인 (GetPawnViewLocation을 쓰기 위함)
	APawn* OwnerPawn = Cast<APawn>(OwnerActor);
	if (!OwnerPawn) return;

	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// 1. 씬 컴포넌트의 월드 위치와 전방 벡터(Forward Vector)를 직접 사용
	FVector TraceStart = TraceAnchorComponent->GetComponentLocation();
	FVector TraceDirection = TraceAnchorComponent->GetForwardVector();

	// 2. 끝점 계산
	FVector TraceEnd = TraceStart + (TraceDirection * InteractionCheckDistance);


	// 4. 디버그 라인 그리기 (설정값이 true일 때만)
	if (bShowDebugLine)
	{
		// 충돌 시 초록색, 비충돌 시 빨간색으로 표시
		FColor DebugColor = InteractionData.CurrentInteractable ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugColor, false, InteractionCheckFrequency, 0, 1.0f);
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);

	FHitResult TraceHit;
	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}
			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}
	NoInteractableFound();
}

void UInteractionComponent::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) EndInteract();

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	TargetInteractable->BeginFocus();

	// 델리게이트 브로드캐스트 (플레이어의 HUD 업데이트용)
	if (OnInteractableFound.IsBound())
	{
		OnInteractableFound.Broadcast(&TargetInteractable->InteractableData);
	}
}

void UInteractionComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		// 델리게이트 브로드캐스트
		if (OnInteractableLost.IsBound())
		{
			OnInteractableLost.Broadcast();
		}

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void UInteractionComponent::BeginInteract()
{
	if (bEnableAutoCheck) PerformInteractionCheck();

	if (InteractionData.CurrentInteractable && IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->BeginInteract();

		if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
		{
			Interact();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle_Interaction, this, &UInteractionComponent::Interact,
				TargetInteractable->InteractableData.InteractionDuration, false
			);
		}
	}
}

void UInteractionComponent::EndInteract()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject())) TargetInteractable->EndInteract();
}

void UInteractionComponent::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		//TargetInteractable->Interact(GetOwner()); // This 대신 Owner 전달

		// Owner를 IInteractorInterface로 캐스팅 시도
		if (IInteractorInterface* Interactor = Cast<IInteractorInterface>(GetOwner()))
		{
			// 성공 시 인터페이스 전달
			TargetInteractable->Interact(Interactor);
		}
	}
}

bool UInteractionComponent::IsInteracting() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction);
}

// AI가 문에 충돌했을 때 트레이스 없이 바로 상호작용하기 위한 유틸 함수
void UInteractionComponent::TriggerInteractionWith(AActor* TargetActor)
{
	if (TargetActor && TargetActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		FoundInteractable(TargetActor);
		BeginInteract();
	}
}