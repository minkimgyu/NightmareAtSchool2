// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionInterface.h"
#include "UserInterface/MainHUD.h"

// PlayerCharacter.h 상단 include 추가
#include "Flashlight/FlashlightComponent.h" // 경로 확인 필수!

#include "Components/InventoryComponent.h"

#include "PlayerCharacter.generated.h"

class AMainPlayerController;
class AMainHUD;
class UInventoryComponent;
class UItemBase;
class UCameraComponent;
class USpotLightComponent;
struct FTimerHandle;

/**
 * ĳ������ ��ȣ�ۿ� ���� �� �����͸� �����ϴ� ����ü�Դϴ�.
 */
USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	/** �⺻ ������ */
	FInteractionData()
		: CurrentInteractable(nullptr) // ���� ��ȣ�ۿ� ���� ���͸� nullptr�� �ʱ�ȭ
		, LastInteractionCheckTime(0.0f) // ������ ��ȣ�ۿ� üũ �ð ��� 0.0f�� �ʱ�ȭ
	{
	}

	/** ���� �÷��̾ ��ȣ�ۿ� ���� ������ �ٶ󺸰ų� ���� ������ �ִ� ���� ������ */
	UPROPERTY()
	AActor* CurrentInteractable;

	/** ���������� ��ȣ�ۿ� ���� ���θ� üũ�� ���� �ð� (������ ������ �ƴ� �ð� ��� ����ȭ�� ����) */
	UPROPERTY()
	float LastInteractionCheckTime;
};

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
class NIGHTMAREATSCHOOL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//=====================================================================
	// �Լ�
	//=====================================================================
	// Sets default values for this character's properties
	APlayerCharacter();
	void ToggleMenu();
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }

	void UpdateInteractionWidget() const;
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	// ... (기존 public 함수)

	// 새로운 public 함수
	/** 현재 스프린트 지속 시간을 반환합니다. UI 업데이트에 사용됩니다. */
	FORCEINLINE float GetCurrentSprintDuration() const { return CurrentSprintDuration; }
	/** 최대 스프린트 지속 시간을 반환합니다. UI 업데이트에 사용됩니다. */
	FORCEINLINE float GetMaxSprintDuration() const { return MaxSprintDuration; }

private:
	//=====================================================================
	// �Լ�
	//=====================================================================
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void StartSprint();
	void StopSprint();

	void ClearSprintCooldownTimer();

	void ToggleCrouch();
	void ToggleFlashlight();

	// 새로운 private 함수
	/** 스프린트 타이머 업데이트 (틱마다 호출) */
	void UpdateSprintDuration(float DeltaTime);
	/** 스프린트가 완전히 소진되었을 때, 쿨다운 시작 */
	void StartSprintCooldown();
	/** 스프린트 사용 가능 여부를 확인합니다. */
	bool CanSprint() const;

protected:

	//=====================================================================
	// ����
	//=====================================================================
	UPROPERTY()
	AMainHUD* HUD;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	EPlayerActionState PlayerActionState;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	EPlayerPostureState PlayerPostureState;



	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	//class USpotLightComponent* Flashlight;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	//bool bFlashlightOn;


	// 기존 USpotLightComponent* Flashlight; 를 아래로 교체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "New_Flashlight")
	UFlashlightComponent* FlashlightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	USpotLightComponent* FlashlightMesh123; // 시각적 라이트 추가

	// 컴포넌트 내부에 직접 콜라이더 생성
	UPROPERTY(VisibleAnywhere, Category = "Flashlight")
	USphereComponent* DetectionSphere123;


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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//bool bIsCrouching;



	/** * ���� ��ȣ�ۿ� ����� �Ǵ� ��ü�� �������̽��� ���� �����մϴ�.
	* IIInteractionInterface �������̽��� ������ ��� ���͸� ������� �� �� �ֽ��ϴ�.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<class IInteractionInterface> TargetInteractable;


	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	/** ��ȣ�ۿ� ���� ���θ� üũ�ϴ� �� (�� ����) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Interaction")
	float InteractionCheckFrequency;

	/** ��ȣ�ۿ� üũ�� ������ �ִ� �Ÿ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Interaction")
	float InteractionCheckDistance;

	/** ��ȣ�ۿ� Ÿ�̸Ӹ� �����ϴ� �ڵ� */
	FTimerHandle TimerHandle_Interaction;

	/** ���� ��ȣ�ۿ� �����͸� �����ϴ� ����ü */
	FInteractionData InteractionData;

	//=====================================================================
	// �Լ�
	//=====================================================================


	void SetState(EPlayerActionState ActionState);
	void SetState(EPlayerPostureState PostureState);

	// 웅크리기 액션 처리 함수
	void ActCrouch(float DeltaTime);


	// ���⼭ ȣ���� �Լ���
	// IInteractionInterface�� �Լ��� ��������ش.

	/** ��ȣ�ۿ� ���� ��ü�� �ֺ��� �ִ��� �ֱ������� üũ�ϴ� �Լ� */
	void PerformInteractionCheck();

	/** ��ȣ�ۿ� ������ ���ο� ��ü�� �߰����� �� ȣ��Ǵ� �Լ� */
	void FoundInteractable(AActor* NewInteractable);

	/** ��ȣ�ۿ� ���� ��ü�� ã�� ���߰ų� ������ ����� �� ȣ��Ǵ� �Լ� */
	void NoInteractableFound();

	/** ��ȣ�ۿ� ���� ������ �����ϴ� �Լ� */
	void BeginInteract();

	/** ��ȣ�ۿ� ���� ������ �����ϴ� �Լ� */
	void EndInteract();

	/** ��ȣ�ۿ��� ������ ó���ϴ� �Լ� (��: ������ �ݱ�, �� ����) */
	void Interact();


	//=====================================================================
	// �Լ�
	//=====================================================================
	// 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
