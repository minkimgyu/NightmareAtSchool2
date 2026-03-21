// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"


#include "InteractionInterface.h"
#include "InteractionComponent.generated.h"

// UI 업데이트 등을 외부에 알리기 위한 델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractableFoundSignature, FInteractableData*);
DECLARE_MULTICAST_DELEGATE(FOnInteractableLostSignature);

class IInteractionInterface;

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f) {}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 외부(Player나 AI)에서 호출할 수 있는 상호작용 액션 */
	void BeginInteract();
	void EndInteract();
	void Interact();

	bool IsInteracting() const;

	// 외부와 소통할 델리게이트 (UI 연동 등)
	FOnInteractableFoundSignature OnInteractableFound;
	FOnInteractableLostSignature OnInteractableLost;

	// AI를 위해 강제로 특정 액터와 상호작용할 수 있는 기능 추가
	void TriggerInteractionWith(AActor* TargetActor);

	// 외부(PlayerCharacter)에서 컴포넌트를 넘겨줄 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InitializeInteraction(USceneComponent* InTraceAnchor);

	// 현재 타겟 액터를 반환하는 게터 (BT Task에서 사용)
	AActor* GetCurrentInteractable() const { return InteractionData.CurrentInteractable; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionCheckFrequency = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionCheckDistance = 225.0f;

	/** AI는 매 프레임 라인트레이스를 할 필요가 없으므로 자동 체크 여부를 설정합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bEnableAutoCheck = true;

	// 시점 위치를 보정하기 위한 오프셋 (X: 앞뒤, Y: 좌우, Z: 상하)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FVector InteractionCheckOffset = FVector::ZeroVector;

	// 디버그 라인을 표시할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bShowDebugLine = true;

	// 레이캐스트의 기준이 될 씬 컴포넌트
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USceneComponent> TraceAnchorComponent;

private:
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();

	UPROPERTY()
	TScriptInterface<class IInteractionInterface> TargetInteractable;

	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

};