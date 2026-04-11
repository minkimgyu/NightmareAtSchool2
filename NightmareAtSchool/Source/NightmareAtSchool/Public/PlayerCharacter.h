// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "InteractionInterface.h"
#include "InteractorInterface.h"


#include "UserInterface/MainHUD.h"

// PlayerCharacter.h 상단 include 추가
#include "Flashlight/FlashlightComponent.h" // 경로 확인 필수!

#include "Components/InventoryComponent.h"

#include "PlayerCharacter.generated.h"

// 헤더 상단에 컴포넌트 추가

class UQuestManagerComponent;
class UInteractionComponent;
class AMainPlayerController;
class AMainHUD;
class UInventoryComponent;
class UItemBase;
class UCameraComponent;
class UJoystickWidget;
class UPlayerInteractionWidget;
class USpotLightComponent;
struct FTimerHandle;

///**
// * ĳ������ ��ȣ�ۿ� ���� �� �����͸� �����ϴ� ����ü�Դϴ�.
// */
//USTRUCT(BlueprintType)
//struct FInteractionData
//{
//	GENERATED_BODY()
//
//	/** �⺻ ������ */
//	FInteractionData()
//		: CurrentInteractable(nullptr) // ���� ��ȣ�ۿ� ���� ���͸� nullptr�� �ʱ�ȭ
//		, LastInteractionCheckTime(0.0f) // ������ ��ȣ�ۿ� üũ �ð ��� 0.0f�� �ʱ�ȭ
//	{
//	}
//
//	/** ���� �÷��̾ ��ȣ�ۿ� ���� ������ �ٶ󺸰ų� ���� ������ �ִ� ���� ������ */
//	UPROPERTY()
//	AActor* CurrentInteractable;
//
//	/** ���������� ��ȣ�ۿ� ���� ���θ� üũ�� ���� �ð� (������ ������ �ƴ� �ð� ��� ����ȭ�� ����) */
//	UPROPERTY()
//	float LastInteractionCheckTime;
//};

UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	Sprint UMETA(DisplayName = "Sprint"),
	Walk UMETA(DisplayName = "Walk")
};

UENUM(BlueprintType)
enum class EPlayerPostureState : uint8
{
	Stand UMETA(DisplayName = "Stand"),
	Crouch UMETA(DisplayName = "Crouch"),
};


UCLASS()
class NIGHTMAREATSCHOOL_API APlayerCharacter : public ACharacter, public IInteractorInterface
{
	GENERATED_BODY()

public:
	//=====================================================================
	// �Լ�
	//=====================================================================
	// Sets default values for this character's properties
	APlayerCharacter();

	// Engine의 TakeDamage 함수를 오버라이드합니다.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 데미지 처리 후 실행할 로직 (애니메이션 재생 등)
	void HandleDamage();

	// 사망 처리 함수
	void HandleDeath();

	UFUNCTION(BlueprintCallable)
	void ToggleMenu();


	UFUNCTION(BlueprintCallable)
	void StartSprint();

	UFUNCTION(BlueprintCallable)
	void StopSprint();

	void ClearSprintCooldownTimer();

	UFUNCTION(BlueprintCallable)
	void ToggleCrouch();

	UFUNCTION(BlueprintCallable)
	void ToggleFlashlight();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 새롭게 추가한 인터페이스 함수
	/////////////////////////////////////////////////////////////////////////


	// IInteractorInterface 구현 (정확한 시그니처 일치 필요)
	virtual AActor* GetInteractorActor() override { return this; }

	// 로그에서 경고가 났던 부분: const 유무를 인터페이스와 일치시켜야 함
	virtual UInventoryComponent* GetInventory() const override { return PlayerInventory; }

	// PlayerCharacter.h 에 선언
	virtual void UpdateInteractionWidget(FInteractableData* InteractableData) override;

	/////////////////////////////////////////////////////////////////////////



	//void UpdateInteractionWidget() const;
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	// ... (기존 public 함수)

	// 새로운 public 함수
	/** 현재 스프린트 지속 시간을 반환합니다. UI 업데이트에 사용됩니다. */
	FORCEINLINE float GetCurrentSprintDuration() const { return CurrentSprintDuration; }
	/** 최대 스프린트 지속 시간을 반환합니다. UI 업데이트에 사용됩니다. */
	FORCEINLINE float GetMaxSprintDuration() const { return MaxSprintDuration; }


	//조이스틱 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UJoystickWidget* JoystickWidgetPtr;

private:
	//=====================================================================
	// �Լ�
	//=====================================================================
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	// 새로운 private 함수
	/** 스프린트 타이머 업데이트 (틱마다 호출) */
	void UpdateSprintDuration(float DeltaTime);
	/** 스프린트가 완전히 소진되었을 때, 쿨다운 시작 */
	void StartSprintCooldown();
	/** 스프린트 사용 가능 여부를 확인합니다. */
	bool CanSprint() const;

	//모바일용
	void MoveUseJoystick();


protected:

	// 캐릭터의 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	/** 자동 회복 관련 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegenRate = 10.0f; // 초당 회복량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float RegenDelay = 3.0f; // 피격 후 회복 시작까지 대기 시간

	bool bCanRegen = false; // 현재 회복 가능한 상태인지 확인

	FTimerHandle TimerHandle_RegenDelay; // 회복 대기용 타이머 핸들

	/** 회복 시작을 위한 함수 */
	void StartHealthRegen();

	// 이동할 게임 오버 레벨의 이름 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName GameOverLevelName = TEXT("L_GameOver");




	/** 뛸 때 소음 크기 (보통 1.0 이상으로 설정하여 더 멀리 퍼지게 함) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | AI")
	float SprintNoiseLoudness = 1.0f;

	/** 소음이 전달되는 범위 배수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | AI")
	float NoiseMaxRange = 1000.0f;

	/** 디버그 서클 표시 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Debug")
	bool bShowFootstepDebug = true;

	/** 디버그 서클이 유지되는 시간 (초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Debug")
	float DebugCircleLifeTime = 1.0f;

	/** 디버그 서클의 색상 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Debug")
	FColor DebugCircleColor = FColor::Red;




	/** 왼발 소리 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FootstepSound_Left;

	/** 오른발 소리 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FootstepSound_Right;

	/** 걷기 발소리 간격 (기본 0.55초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float WalkFootstepDelay = 0.55f;

	/** 뛰기 발소리 간격 (기본 0.3초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float RunFootstepDelay = 0.3f;

	/** 현재 적용 중인 발소리 딜레이 저장용 */
	float CurrentFootstepDelay = 0.0f;

	/** 현재 어떤 발소리를 낼지 (true: 왼발, false: 오른발) */
	bool bIsLeftFoot = true;

	/** 발소리 재생 함수 */
	void PlayFootstepSound();

	/** 발소리 타이머 핸들러 */
	FTimerHandle FootstepTimerHandle;



	// 새로 추가할 상호작용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComp;

	// UI 델리게이트 연동용 콜백 함수
	void OnInteractableFound(FInteractableData* InteractableData);
	void OnInteractableLost();

	//=====================================================================
	// ����
	//=====================================================================
	UPROPERTY()
	AMainHUD* HUD;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	EPlayerActionState PlayerActionState;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	EPlayerPostureState PlayerPostureState;




	// 기존 USpotLightComponent* Flashlight; 를 아래로 교체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "New_Flashlight")
	UFlashlightComponent* FlashlightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	USpotLightComponent* FlashlightMesh123; // 시각적 라이트 추가

	// 컴포넌트 내부에 직접 콜라이더 생성
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flashlight")
	USphereComponent* DetectionSphere123;

	// 컴포넌트 내부에 직접 콜라이더 생성
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	USceneComponent* InteractionSight;

	// 컴포넌트 내부에 직접 콜라이더 생성
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	UQuestManagerComponent* QuestManagerCom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCamera;



	// 새로운 UPROPERTY 변수 (스프린트 타이머 관련)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float MaxSprintDuration = 5.0f; // 최대 스프린트 지속 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float SprintCooldownDuration = 2.0f; // 스프린트 소진 후 쿨다운 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float SprintRechargeRate = 1.0f; // 걷기/멈춤 상태일 때 초당 회복량

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Sprint")
	float CurrentSprintDuration; // 현재 남은 스프린트 시간

	// 새로운 FTimerHandle
	FTimerHandle TimerHandle_SprintCooldown; // 스프린트 쿨다운 타이머







	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StandingHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchingHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchInterpSpeed; // 부드러운 속도 설정


	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;


	//=====================================================================
	// �Լ�
	//=====================================================================


	void SetState(EPlayerActionState ActionState);
	void SetState(EPlayerPostureState PostureState);

	// 웅크리기 액션 처리 함수
	void ActCrouch(float DeltaTime);


	//=====================================================================
	// �Լ�
	//=====================================================================
	// 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
