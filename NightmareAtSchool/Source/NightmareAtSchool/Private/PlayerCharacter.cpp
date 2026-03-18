// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UserInterface/MainHUD.h"
#include "TimerManager.h"

#include "Camera/CameraComponent.h" // 카메라
#include "Components/SpotLightComponent.h" // 손전등

#include "Components/CapsuleComponent.h" // ⬅️ 이 줄을 추가합니다.

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "InputActionValue.h"
#include "Pickup.h"

#include "Components/InventoryComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	CurrentSprintDuration = MaxSprintDuration;

	CrouchSpeedMultiplier = 0.5f;
	CrouchSpeed = NormalSpeed * CrouchSpeedMultiplier;

	StandingHalfHeight = 96.f;
	CrouchingHalfHeight = 48.f;
	CrouchInterpSpeed = 8.f; // 부드러운 속도 설정

	SetState(EPlayerActionState::Walk);
	SetState(EPlayerPostureState::Stand);

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;

	// 1) Mesh 먼저 붙임
	GetMesh()->SetupAttachment(RootComponent);

	// 2) 카메라 만들기
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
	FirstPersonCamera->bUsePawnControlRotation = true;  // 마우스 회전 그대로 반영

	// 3) 플래시라이트 컴포넌트 만들기 (기존 USpotLightComponent 생성 코드 삭제)
	FlashlightComponent = CreateDefaultSubobject<UFlashlightComponent>(TEXT("FlashlightCom"));
	FlashlightComponent->SetupAttachment(FirstPersonCamera); // 붙이기

	// 1. 콜라이더 생성 및 설정
	DetectionSphere123 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp123"));
	FlashlightMesh123 = CreateDefaultSubobject<USpotLightComponent>(TEXT("LightComp123"));

	// 이 SceneComponent(FlashlightComponent) 밑에 콜라이더를 붙임
	DetectionSphere123->SetupAttachment(FirstPersonCamera);
	FlashlightMesh123->SetupAttachment(FirstPersonCamera);
}

void APlayerCharacter::ToggleFlashlight()
{
	if (FlashlightComponent)
	{
		// 컴포넌트 내부의 Toggle 함수를 호출하여 빛 + 판정 동시 제어
		FlashlightComponent->ToggleFlashlight();

		// 현재 상태 로그 출력
		bool bCurrentState = FlashlightComponent->bIsOn;
		UE_LOG(LogTemp, Log, TEXT("Flashlight: %s"), bCurrentState ? TEXT("ON") : TEXT("OFF"));
	}
}

void APlayerCharacter::DropItem(UItemBase* ItemToDrop, int32 QuantityToDrop)
{
	// 아이템 드랍 가능 여부 확인 (인벤토리에서 매칭되는 아이템 찾기)
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		// 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		// 충돌 처리 설정: 가능한 경우 조정하지만, 항상 스폰 시도
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 아이템이 스폰될 위치 계산: 현재 액터 위치에서 앞쪽 방향으로 90.0f 떨어진 곳
		const FVector SpawnLocation = (GetActorLocation() + (GetActorForwardVector() * 90.0f));
		// 스폰 트랜스폼 생성: 현재 액터의 회전과 계산된 스폰 위치를 사용
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		// 인벤토리에서 아이템 제거
		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		// 픽업(Pickup) 액터 스폰: 월드에 APickup 클래스의 인스턴스를 스폰
		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		// 스폰된 픽업 액터 초기화 (어떤 아이템이 얼마나 제거되었는지 전달)
		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		// 아이템이 null이거나 인벤토리에서 찾을 수 없는 경우 경고 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return; // ��Ʈ�ѷ��� ���� ��� �������� ����

	const FVector2D MoveInput = value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MoveInput.X);
	AddMovementInput(GetActorRightVector(), MoveInput.Y);
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::ToggleCrouch()
{
	EPlayerPostureState NewPostureState;

	if (PlayerPostureState == EPlayerPostureState::Crouch) NewPostureState = EPlayerPostureState::Stand;
	else NewPostureState = EPlayerPostureState::Crouch;

	SetState(NewPostureState);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 로직 컴포넌트에게 조작해야 할 부품(라이트, 구역)을 넘겨줌
	if (FlashlightComponent)
	{
		FlashlightComponent->InitializeFlashlight(FlashlightMesh123, DetectionSphere123);
		FlashlightComponent->SetFlashlightState(false); // 시작은 꺼짐
	}

	HUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void APlayerCharacter::ActCrouch(float DeltaTime)
{
	float TargetHeight = 0;

	switch (PlayerPostureState)
	{
		case EPlayerPostureState::Crouch:
			TargetHeight = CrouchingHalfHeight;
			break;
		default:
			TargetHeight = StandingHalfHeight;
			break;
	}

	float CurrentHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	// 목표 높이와 거의 같으면 계산 중단
	if (FMath::IsNearlyEqual(CurrentHeight, TargetHeight, 0.5f))
	{
		return;
	}

	float NewHeight = FMath::FInterpTo(CurrentHeight, TargetHeight, DeltaTime, CrouchInterpSpeed);
	float Delta = CurrentHeight - NewHeight;

	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight, true);
	GetMesh()->AddLocalOffset(FVector(0, 0, -Delta));
}

void APlayerCharacter::UpdateSprintDuration(float DeltaTime)
{
	// 스프린트 중일 경우
	if (PlayerActionState == EPlayerActionState::Sprint)
	{
		CurrentSprintDuration -= DeltaTime;


		UE_LOG(LogTemp, Log, TEXT("UpdateSprintDuration: %f"), CurrentSprintDuration);

		// 스프린트 시간이 다 떨어지면, 강제로 걷기 상태로 전환하고 쿨다운 시작
		if (CurrentSprintDuration <= 0.0f)
		{
			
			CurrentSprintDuration = 0.0f;
			StopSprint(); // StopSprint 내부에서 SetState(Walk) 호출
			StartSprintCooldown();
		}
	}
	// 걷기(Walk) 또는 멈춤(Idle) 상태일 경우 (쿨다운 중이 아닐 때만 회복)
	else if (!GetWorldTimerManager().IsTimerActive(TimerHandle_SprintCooldown))
	{
		CurrentSprintDuration += DeltaTime * SprintRechargeRate;
		CurrentSprintDuration = FMath::Min(CurrentSprintDuration, MaxSprintDuration);

		if (FMath::IsNearlyEqual(MaxSprintDuration, CurrentSprintDuration))
		{
			HUD->HideSprintBar();
		}
	}

	// UI 업데이트 (HUD에서 CurrentSprintDuration을 직접 가져가서 표시할 수도 있습니다.)
	// 이 예시에서는 HUD 업데이트 로직은 생략합니다. (HUD 위젯에서 Tick을 사용하는 것이 일반적입니다.)
}

bool APlayerCharacter::CanSprint() const
{
	// 쿨다운 타이머가 활성화되지 않았고, 스프린트 시간이 0보다 커야 스프린트 가능
	return !GetWorldTimerManager().IsTimerActive(TimerHandle_SprintCooldown) && (CurrentSprintDuration > 0.0f);
}

void APlayerCharacter::StartSprint()
{
	// ⭐ 스프린트 가능 여부 체크 로직 추가
	if (CanSprint())
	{
		HUD->ShowSprintBar();
		SetState(EPlayerActionState::Sprint);
	}
}

void APlayerCharacter::StopSprint()
{
	SetState(EPlayerActionState::Walk);
}

void APlayerCharacter::ClearSprintCooldownTimer()
{
	// 쿨다운 타이머를 해제합니다.
	GetWorldTimerManager().ClearTimer(TimerHandle_SprintCooldown);
	// 필요하다면, 쿨다운이 끝났음을 알리는 로그나 UI 업데이트를 추가할 수 있습니다.
	UE_LOG(LogTemp, Log, TEXT("Sprint Cooldown Finished."));
}

void APlayerCharacter::StartSprintCooldown()
{
	// 쿨다운 타이머 설정
	GetWorldTimerManager().SetTimer(
		TimerHandle_SprintCooldown,
		this,
		// 타이머가 만료되면 쿨다운 타이머를 클리어합니다.
		&APlayerCharacter::ClearSprintCooldownTimer, // 실행할 멤버 함수 포인터
		SprintCooldownDuration,
		false // 반복하지 않음
	);

	// 쿨다운 종료 시 TimerHandle_SprintCooldown을 수동으로 해제하는 함수를 만들 수도 있습니다.
	// 여기서는 TimerHandle을 그대로 두고, CanSprint()에서 IsTimerActive로 체크합니다.
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	// ⭐ 스프린트 지속 시간 업데이트 로직 추가
	UpdateSprintDuration(DeltaTime);

	ActCrouch(DeltaTime);
}

//=====================================================================
// ��ȣ �ۿ� �Լ�
//=====================================================================

void APlayerCharacter::SetState(EPlayerActionState ActionState)
{
	if (PlayerActionState == ActionState) return;  // 같은 상태 전이 불가능

	// 만약 앉은 상태라면 달리기 변환 못함
	if (PlayerPostureState == EPlayerPostureState::Crouch &&
		ActionState == EPlayerActionState::Sprint) return;

	// ⭐ 스프린트 상태로의 전환을 시도할 때, 스프린트 가능 여부 한 번 더 체크 (StartSprint에서 이미 했지만 방어 코드)
	if (ActionState == EPlayerActionState::Sprint && CurrentSprintDuration <= 0.0f)
	{
		// 스프린트 불가능 상태에서는 Walk로 설정 유지
		return;
	}


	PlayerActionState = ActionState;
	UE_LOG(LogTemp, Warning, TEXT("Change ActionState"));

	if (GetCharacterMovement())
	{
		switch (PlayerActionState)
		{
			case EPlayerActionState::Sprint:
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
				break;
			case EPlayerActionState::Walk:
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
				break;

			default:
				break;
		}
	}
}

void APlayerCharacter::SetState(EPlayerPostureState PostureState)
{
	if (PlayerPostureState == PostureState) return; // 같은 상태 전이 불가능

	// 만약 달리는 상태라면 자세 변환 못함
	if (PlayerActionState == EPlayerActionState::Sprint &&
		PostureState == EPlayerPostureState::Crouch) return;

	PlayerPostureState = PostureState;
	UE_LOG(LogTemp, Warning, TEXT("Change PostureState"));

	if (GetCharacterMovement())
	{
		switch (PlayerPostureState)
		{
			case EPlayerPostureState::Crouch:
				GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
				break;
			case EPlayerPostureState::Stand:
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
				break;

			default:
				break;
		}
	}
}

//=====================================================================
// ��ȣ �ۿ� �Լ�
//=====================================================================

void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// 1. Ʈ���̽� ������ ����
	FVector TraceStart = GetPawnViewLocation();

	// 2. Ʈ���̽� ���� ����
	// ���� �þ� ȸ��(GetViewRotation())�� ���� ���Ϳ� ��ȣ�ۿ� �Ÿ�(InteractionCheckDistance)�� ���Ͽ� ������ ����մϴ�.
	FVector TraceEnd = TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance);

	// ������ �ٶ󺸴��� ���� �ľ�
	//double LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	//if (LookDirection > 0)
	//{
		// ����, ���� �ð�, ���� �켱����, ����
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		// 3. ���� �Ķ���� ����
		FCollisionQueryParams QueryParams;
		// Ʈ���̽��� �����ϴ� ����(�ַ� �÷��̾� ĳ����)�� �����Ͽ� �ڱ� �ڽŰ� �浹���� �ʵ��� �մϴ�.
		QueryParams.AddIgnoredActor(this);

		// 4. Ʈ���̽� ��� ����ü
		FHitResult TraceHit;

		// 5. ���� Ʈ���̽� ����
		// GetWorld()->LineTraceSingleByChannel(���, ������, ����, �浹 ä��, ���� �Ķ����)
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			// 6. Ʈ���̽��� �ɸ� ���Ͱ� ��ȣ�ۿ� �������̽��� �����ߴ��� Ȯ��
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				// 7. Ʈ���̽��� �ɸ� ��ü�� ���� ��ü�� �ƴϰų� (���ο� ��ȣ�ۿ� ���)
				//    ���� �Ÿ��� ��ȣ�ۿ� �Ÿ�(InteractionCheckDistance) �̳����� Ȯ��
				//const float Distance = (TraceStart - TraceHit.ImpactPoint).Size(); // �浹 ���������� �Ÿ� ���
				// && Distance <= InteractionCheckDistance

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					// ���ο� ��ȣ�ۿ� ���� ��ü�� �߰����� ���� ó�� �Լ� ȣ��
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				// 8. Ʈ���̽��� �ɸ� ��ü�� ���� ��ȣ�ۿ� ���� ��ü�� �����ϴٸ�
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					// Ư���� ���� ��ȭ ���� �Լ� ���� (��ȣ�ۿ� ���� ���� ����)
					return;
				}
			}
		}
	//}

	NoInteractableFound();
}

void APlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	UE_LOG(LogTemp, Warning,
		TEXT("name: %s, action: %s"),
		*TargetInteractable->InteractableData.Name.ToString(),
		*TargetInteractable->InteractableData.Action.ToString()
	);

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	TargetInteractable->BeginFocus();
}


void APlayerCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}


void APlayerCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void APlayerCharacter::NoInteractableFound()
{
	// 1. ��ȣ�ۿ� ���̾��ٸ� Ÿ�̸Ӹ� �����մϴ�.
	if (IsInteracting())
	{
		// Ÿ�̸Ӱ� Ȱ��ȭ�Ǿ� ������ �ش� Ÿ�̸Ӹ� �����ϰ� �ڵ��� �����մϴ�.
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
		// ����: ��ũ�������� ���� ĸó(&)�� ������, FTimerHandle ������ ���� �����ϴ� ���� �Ϲ����Դϴ�.
	}

	// 2. ���� ��ȣ�ۿ� ����� �־��ٸ� ��Ŀ���� �����մϴ�.
	if (InteractionData.CurrentInteractable)
	{
		// TargetInteractable�� IInteractionInterface�� ������ TScriptInterface�Դϴ�.
		// GetObject()�� ���� UObject �����͸� ������ �� IsValid()�� ��ȿ���� �˻��մϴ�.
		if (IsValid(TargetInteractable.GetObject()))
		{
			// �������̽� �Լ��� EndFocus()�� ȣ���Ͽ� ��ü�� ��Ŀ�� ���¸� �����մϴ�.
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		// // ��ȣ�ۿ� ������ HUD���� ����� ���� (�ּ����� ������)
		// hide interaction widget on the HUD

		// 3. �����͸� �ʱ�ȭ�մϴ�.
		// CurrentInteractable ���� �����͸� nullptr�� �ʱ�ȭ�մϴ�.
		InteractionData.CurrentInteractable = nullptr;

		// TargetInteractable �������̽� �����͸� nullptr�� �ʱ�ȭ�մϴ�.
		TargetInteractable = nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	// 1. ��ȣ�ۿ��� ������ �������� ���°� ������� �ʾҴ��� Ȯ���մϴ�.
	// (��: Line Trace�� �ٽ� �����Ͽ� ���� �ٶ󺸴� ��ü�� ������ ��ȣ�ۿ� �������� Ȯ��)
	PerformInteractionCheck();

	// 2. InteractionData�� ��ȿ�� CurrentInteractable�� �ִ��� Ȯ���մϴ�.
	if (InteractionData.CurrentInteractable)
	{
		// 3. TargetInteractable �������̽� �����Ͱ� ��ȿ���� Ȯ���մϴ�.
		if (IsValid(TargetInteractable.GetObject()))
		{
			// �������̽� �Լ��� BeginInteract()�� ȣ���Ͽ� ��ȣ�ۿ� ��� ��ü�� ��ȣ�ۿ� ������ �˸��ϴ�.
			TargetInteractable->BeginInteract();

			// 4. ��ȣ�ۿ뿡 ���� �ð��� �ʿ����� Ȯ���մϴ�.
			// FMath::IsNearlyZero�� �ε��Ҽ���(float) ���� 0�� ������� Ȯ���ϴ� ������ ����Դϴ�.
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				// ���� �ð��� 0�� �����ٸ� (��� ��ȣ�ۿ�)
				Interact(); // Interact() �Լ��� ��� ȣ���մϴ�. (��ŸŸ�� 0.0f ����)
			}
			else
			{
				// ���� �ð��� �ִٸ� (��� ������ �ϴ� ��ȣ�ۿ�)
				// Ÿ�̸Ӹ� �����Ͽ� ���� �ð� �Ŀ� Interact() �Լ��� �����ϵ��� �����մϴ�.
				GetWorldTimerManager().SetTimer(
					TimerHandle_Interaction,                                       // FTimerHandle: Ÿ�̸Ӹ� ������ �ڵ�
					this,                                                          // InObj: �Լ��� ������ ��ü (���� Ŭ���� �ν��Ͻ�)
					&APlayerCharacter::Interact,                                     // InRate: ������ �Լ� ������
					TargetInteractable->InteractableData.InteractionDuration,      // Rate: Ÿ�̸� �ֱ� (���⼭�� ��ȣ�ۿ� ���� �ð�)
					false                                                          // InbLoop: �ݺ� ���� (false: �� ���� ����)
				);
			}
		}
	}
}

void APlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}


void APlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

//=====================================================================
// �Լ�
//=====================================================================

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopSprint
				);
			}

			if (PlayerController->InteractAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InteractAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::BeginInteract
				);

				EnhancedInput->BindAction(
					PlayerController->InteractAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::EndInteract
				);
			}

			if (PlayerController->ToggleMenuAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ToggleMenuAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ToggleMenu
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ToggleCrouch // 함수 이름 변경
				);
			}

			if (PlayerController->FlashlightAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FlashlightAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ToggleFlashlight // 함수 이름 변경
				);
			}
		}
	}
}