// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UserInterface/MainHUD.h"
#include "TimerManager.h"

#include "Kismet/GameplayStatics.h" // PlaySoundAtLocation 사용을 위해 필요

#include "Quest/QueseManagerCompoent.h"

#include "Camera/CameraComponent.h" // 카메라
#include "Components/SpotLightComponent.h" // 손전등

#include "Components/CapsuleComponent.h" // ⬅️ 이 줄을 추가합니다.

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "InputActionValue.h"
#include "Pickup.h"

#include "Components/InventoryComponent.h"
#include "Components/InteractionComponent.h" // 인클루드 추가

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

	QuestManagerCom = CreateDefaultSubobject<UQuestManagerComponent>(TEXT("QuestManagerCom123"));

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	// bEnableAutoCheck는 기본값이 true이므로 플레이어는 자동 라인트레이스가 작동합니다.

	//InteractionCheckFrequency = 0.1f;
	//InteractionCheckDistance = 225.0f;

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
	InteractionSight = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionSight123"));

	// 이 SceneComponent(FlashlightComponent) 밑에 콜라이더를 붙임
	DetectionSphere123->SetupAttachment(FirstPersonCamera);
	FlashlightMesh123->SetupAttachment(FirstPersonCamera);
	InteractionSight->SetupAttachment(FirstPersonCamera);

	Health = MaxHealth;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 부모 클래스의 기본 로직 실행
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		// 체력 감소
		Health -= ActualDamage;

		// 1. 피격 시 회복 중단 및 타이머 리셋
		bCanRegen = false;

		GetWorldTimerManager().ClearTimer(TimerHandle_RegenDelay);

		// 2. 3초(RegenDelay) 후에 StartHealthRegen 함수 호출 예약
		GetWorldTimerManager().SetTimer(TimerHandle_RegenDelay, this, &APlayerCharacter::StartHealthRegen, RegenDelay, false);

		UE_LOG(LogTemp, Warning, TEXT("Player Hit! Remaining HP: %f"), Health);

		// 데미지 반응 로직 (피격 애니메이션, UI 업데이트 등)
		HandleDamage();

		if (Health <= 0.0f)
		{
			Health = 0.0f;
			HandleDeath();
			// 사망 처리 로직 호출
			UE_LOG(LogTemp, Error, TEXT("Player is Dead!"));
		}

		HUD->UpdateHPUI(Health);
	}

	return ActualDamage;
}

void APlayerCharacter::StartHealthRegen()
{
	if (Health > 0.0f && Health < MaxHealth)
	{
		bCanRegen = true;
		UE_LOG(LogTemp, Log, TEXT("Health Regeneration Started"));
	}
}

void APlayerCharacter::HandleDeath()
{
	// 중복 실행 방지 (사망 처리 중 다시 데미지를 입는 경우 대비)
	Health = 0.0f;

	UE_LOG(LogTemp, Error, TEXT("Player Dead! Loading Game Over Level..."));

	// 1. 입력 중지 (선택 사항: 플레이어가 죽은 뒤 조작 못하게 방지)
	//APlayerController* PC = Cast<APlayerController>(GetController());
	//if (PC)
	//{
	//	DisableInput(PC);
	//}

	// 2. 게임 오버 레벨 로드
	// GameOverLevelName은 에디터에서 생성한 레벨 이름과 정확히 일치해야 합니다.
	UGameplayStatics::OpenLevel(GetWorld(), GameOverLevelName);
}

void APlayerCharacter::HandleDamage()
{
	// SD 도트 캐릭터라면 여기서 피격용 스프라이트 변경이나 
	// 짧은 무적 프레임(Blink 효과) 등을 구현하면 좋습니다.
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

	// 컴포넌트의 델리게이트와 HUD 업데이트 로직 바인딩
	if (InteractionComp)
	{
		InteractionComp->OnInteractableFound.AddUObject(this, &APlayerCharacter::OnInteractableFound);
		InteractionComp->OnInteractableLost.AddUObject(this, &APlayerCharacter::OnInteractableLost);

		InteractionComp->InitializeInteraction(InteractionSight);
	}
}

// 콜백 함수 구현
void APlayerCharacter::OnInteractableFound(FInteractableData* InteractableData)
{
	if (HUD) HUD->UpdateInteractionWidget(InteractableData);
}

void APlayerCharacter::OnInteractableLost()
{
	if (HUD) HUD->HideInteractionWidget();
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

void APlayerCharacter::PlayFootstepSound()
{
	// 왼발/오른발 상태에 따라 사운드 선택
	USoundBase* SoundToPlay = bIsLeftFoot ? FootstepSound_Left : FootstepSound_Right;

	if (SoundToPlay)
	{
		// 캐릭터 발 위치 근처에서 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
	}

	// 다음 실행 시 반대 발 소리가 나도록 토글
	bIsLeftFoot = !bIsLeftFoot;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 이동 및 접지 상태 확인
	float Speed = GetVelocity().Size();
	bool bIsMoving = Speed > 10.0f;
	bool bIsOnGround = !GetCharacterMovement()->IsFalling();

	if (bIsMoving && bIsOnGround)
	{
		// 2. 현재 상태(Walk/Sprint)에 따른 목표 딜레이 결정
		float TargetDelay = (PlayerActionState == EPlayerActionState::Sprint) ? RunFootstepDelay : WalkFootstepDelay;

		// 3. 딜레이가 바뀌었거나(걷다 뛰기 시작 등) 타이머가 꺼져있을 때만 타이머 갱신
		if (!FMath::IsNearlyEqual(TargetDelay, CurrentFootstepDelay) || !GetWorldTimerManager().IsTimerActive(FootstepTimerHandle))
		{
			CurrentFootstepDelay = TargetDelay;

			// 기존 타이머가 있다면 초기화하고 새로운 주기로 시작
			GetWorldTimerManager().SetTimer(FootstepTimerHandle, this, &APlayerCharacter::PlayFootstepSound, CurrentFootstepDelay, true);
		}
	}
	else
	{
		// 멈추거나 공중이면 타이머 즉시 해제
		if (GetWorldTimerManager().IsTimerActive(FootstepTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(FootstepTimerHandle);
			CurrentFootstepDelay = 0.0f;
			bIsLeftFoot = true; // 다음에 걸을 때 다시 왼발부터 시작하도록 초기화
		}
	}

	// ⭐ 스프린트 지속 시간 업데이트 로직 추가
	UpdateSprintDuration(DeltaTime);

	// 자동 회복 로직
	if (bCanRegen && Health < MaxHealth)
	{
		Health += HealthRegenRate * DeltaTime;

		// MaxHealth를 넘지 않도록 제한
		Health = FMath::Min(Health, MaxHealth);

		// HUD 업데이트
		if (HUD) HUD->UpdateHPUI(Health);

		// 풀피가 되면 회복 중단
		if (Health >= MaxHealth)
		{
			bCanRegen = false;
			UE_LOG(LogTemp, Log, TEXT("Health Fully Restored"));
		}
	}

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

// PlayerCharacter.cpp 구현
void APlayerCharacter::UpdateInteractionWidget(FInteractableData* InteractableData)
{
	// HUD와 데이터가 유효한지 확인 후 업데이트
	if (HUD && InteractableData)
	{
		HUD->UpdateInteractionWidget(InteractableData);
	}
}

void APlayerCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
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
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Started, InteractionComp, &UInteractionComponent::BeginInteract);
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Completed, InteractionComp, &UInteractionComponent::EndInteract);
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