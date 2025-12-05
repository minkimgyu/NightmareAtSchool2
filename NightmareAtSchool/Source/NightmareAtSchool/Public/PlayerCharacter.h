// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionInterface.h"
#include "UserInterface/MainHUD.h"

#include "PlayerCharacter.generated.h"

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
		, LastInteractionCheckTime(0.0f) // ������ ��ȣ�ۿ� üũ �ð��� 0.0f�� �ʱ�ȭ
	{
	}

	/** ���� �÷��̾ ��ȣ�ۿ� ���� ������ �ٶ󺸰ų� ���� ������ �ִ� ���� ������ */
	UPROPERTY()
	AActor* CurrentInteractable;

	/** ���������� ��ȣ�ۿ� ���� ���θ� üũ�� ���� �ð� (������ ������ �ƴ� �ð� ��� ����ȭ�� ����) */
	UPROPERTY()
	float LastInteractionCheckTime;
};

class AMainPlayerController;
class AMainHUD;

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };


private:
	//=====================================================================
	// �Լ�
	//=====================================================================
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

protected:

	//=====================================================================
	// ����
	//=====================================================================
	UPROPERTY()
	AMainHUD* HUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	/** * ���� ��ȣ�ۿ� ����� �Ǵ� ��ü�� �������̽��� ���� �����մϴ�.
	* IIInteractionInterface �������̽��� ������ ��� ���͸� ������� �� �� �ֽ��ϴ�.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<class IInteractionInterface> TargetInteractable;

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

	// ���⼭ ȣ���� �Լ���
	// IInteractionInterface�� �Լ��� ��������ش�.

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
