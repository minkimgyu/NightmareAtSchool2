// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionInterface.h"

#include "PlayerCharacter.generated.h"

/**
 * 캐릭터의 상호작용 상태 및 데이터를 관리하는 구조체입니다.
 */
USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	/** 기본 생성자 */
	FInteractionData()
		: CurrentInteractable(nullptr) // 현재 상호작용 중인 액터를 nullptr로 초기화
		, LastInteractionCheckTime(0.0f) // 마지막 상호작용 체크 시간을 0.0f로 초기화
	{
	}

	/** 현재 플레이어가 상호작용 범위 내에서 바라보거나 가장 가까이 있는 액터 포인터 */
	UPROPERTY()
	AActor* CurrentInteractable;

	/** 마지막으로 상호작용 가능 여부를 체크한 게임 시간 (프레임 단위가 아닌 시간 기반 최적화를 위함) */
	UPROPERTY()
	float LastInteractionCheckTime;
};

class AMainPlayerController;

UCLASS()
class NIGHTMAREATSCHOOL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//=====================================================================
	// 함수
	//=====================================================================
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };


private:
	//=====================================================================
	// 함수
	//=====================================================================
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

protected:

	//=====================================================================
	// 변수
	//=====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	/** * 현재 상호작용 대상이 되는 객체를 인터페이스를 통해 참조합니다.
	* IIInteractionInterface 인터페이스를 구현한 모든 액터를 대상으로 할 수 있습니다.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<class IInteractionInterface> TargetInteractable;

	/** 상호작용 가능 여부를 체크하는 빈도 (초 단위) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Interaction")
	float InteractionCheckFrequency;

	/** 상호작용 체크를 수행할 최대 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Interaction")
	float InteractionCheckDistance;

	/** 상호작용 타이머를 관리하는 핸들 */
	FTimerHandle TimerHandle_Interaction;

	/** 현재 상호작용 데이터를 저장하는 구조체 */
	FInteractionData InteractionData;

	//=====================================================================
	// 함수
	//=====================================================================

	// 여기서 호출한 함수가
	// IInteractionInterface의 함수를 실행시켜준다.

	/** 상호작용 가능 객체가 주변에 있는지 주기적으로 체크하는 함수 */
	void PerformInteractionCheck();

	/** 상호작용 가능한 새로운 객체를 발견했을 때 호출되는 함수 */
	void FoundInteractable(AActor* NewInteractable);

	/** 상호작용 가능 객체를 찾지 못했거나 범위를 벗어났을 때 호출되는 함수 */
	void NoInteractableFound();

	/** 상호작용 시작 로직을 수행하는 함수 */
	void BeginInteract();

	/** 상호작용 종료 로직을 수행하는 함수 */
	void EndInteract();

	/** 상호작용을 실제로 처리하는 함수 (예: 아이템 줍기, 문 열기) */
	void Interact();

	//=====================================================================
	// 함수
	//=====================================================================
	// 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
