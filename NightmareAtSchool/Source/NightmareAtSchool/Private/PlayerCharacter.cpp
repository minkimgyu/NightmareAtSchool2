// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "InputActionValue.h"

#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return; // 컨트롤러가 없는 경우 진행하지 않음

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

void APlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

//=====================================================================
// 상호 작용 함수
//=====================================================================

void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// 1. 트레이스 시작점 설정
	FVector TraceStart = GetPawnViewLocation();

	// 2. 트레이스 끝점 설정
	// 현재 시야 회전(GetViewRotation())의 정면 벡터에 상호작용 거리(InteractionCheckDistance)를 곱하여 끝점을 계산합니다.
	FVector TraceEnd = TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance);

	// 정면을 바라보는지 여부 파악
	//double LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	//if (LookDirection > 0)
	//{
		// 지속, 유지 시간, 깊이 우선순위, 굵기
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		// 3. 쿼리 파라미터 설정
		FCollisionQueryParams QueryParams;
		// 트레이스를 수행하는 액터(주로 플레이어 캐릭터)는 무시하여 자기 자신과 충돌하지 않도록 합니다.
		QueryParams.AddIgnoredActor(this);

		// 4. 트레이스 결과 구조체
		FHitResult TraceHit;

		// 5. 라인 트레이스 수행
		// GetWorld()->LineTraceSingleByChannel(결과, 시작점, 끝점, 충돌 채널, 쿼리 파라미터)
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			// 6. 트레이스에 걸린 액터가 상호작용 인터페이스를 구현했는지 확인
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				// 7. 트레이스에 걸린 객체가 기존 객체가 아니거나 (새로운 상호작용 대상)
				//    현재 거리가 상호작용 거리(InteractionCheckDistance) 이내인지 확인
				//const float Distance = (TraceStart - TraceHit.ImpactPoint).Size(); // 충돌 지점까지의 거리 계산
				// && Distance <= InteractionCheckDistance

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					// 새로운 상호작용 가능 객체를 발견했을 때의 처리 함수 호출
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				// 8. 트레이스에 걸린 객체가 현재 상호작용 중인 객체와 동일하다면
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					// 특별한 상태 변화 없이 함수 종료 (상호작용 가능 상태 유지)
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

	TargetInteractable->BeginFocus();
}

void APlayerCharacter::NoInteractableFound()
{
	// 1. 상호작용 중이었다면 타이머를 정리합니다.
	if (IsInteracting())
	{
		// 타이머가 활성화되어 있으면 해당 타이머를 중지하고 핸들을 정리합니다.
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
		// 참고: 스크린샷에는 람다 캡처(&)가 있지만, FTimerHandle 변수를 직접 전달하는 것이 일반적입니다.
	}

	// 2. 현재 상호작용 대상이 있었다면 포커스를 해제합니다.
	if (InteractionData.CurrentInteractable)
	{
		// TargetInteractable은 IInteractionInterface를 구현한 TScriptInterface입니다.
		// GetObject()로 실제 UObject 포인터를 가져온 후 IsValid()로 유효성을 검사합니다.
		if (IsValid(TargetInteractable.GetObject()))
		{
			// 인터페이스 함수인 EndFocus()를 호출하여 객체의 포커스 상태를 해제합니다.
			TargetInteractable->EndFocus();
		}

		// // 상호작용 위젯을 HUD에서 숨기는 로직 (주석으로 설명됨)
		// hide interaction widget on the HUD

		// 3. 데이터를 초기화합니다.
		// CurrentInteractable 액터 포인터를 nullptr로 초기화합니다.
		InteractionData.CurrentInteractable = nullptr;

		// TargetInteractable 인터페이스 포인터를 nullptr로 초기화합니다.
		TargetInteractable = nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	// 1. 상호작용을 시작한 시점부터 상태가 변경되지 않았는지 확인합니다.
	// (예: Line Trace를 다시 실행하여 현재 바라보는 객체가 여전히 상호작용 가능한지 확인)
	PerformInteractionCheck();

	// 2. InteractionData에 유효한 CurrentInteractable이 있는지 확인합니다.
	if (InteractionData.CurrentInteractable)
	{
		// 3. TargetInteractable 인터페이스 포인터가 유효한지 확인합니다.
		if (IsValid(TargetInteractable.GetObject()))
		{
			// 인터페이스 함수인 BeginInteract()를 호출하여 상호작용 대상 객체에 상호작용 시작을 알립니다.
			TargetInteractable->BeginInteract();

			// 4. 상호작용에 지속 시간이 필요한지 확인합니다.
			// FMath::IsNearlyZero는 부동소수점(float) 값이 0에 가까운지 확인하는 안전한 방법입니다.
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				// 지속 시간이 0에 가깝다면 (즉시 상호작용)
				Interact(); // Interact() 함수를 즉시 호출합니다. (델타타임 0.0f 전달)
			}
			else
			{
				// 지속 시간이 있다면 (길게 눌러야 하는 상호작용)
				// 타이머를 설정하여 일정 시간 후에 Interact() 함수를 실행하도록 예약합니다.
				GetWorldTimerManager().SetTimer(
					TimerHandle_Interaction,                                       // FTimerHandle: 타이머를 관리할 핸들
					this,                                                          // InObj: 함수를 실행할 객체 (현재 클래스 인스턴스)
					&APlayerCharacter::Interact,                                     // InRate: 실행할 함수 포인터
					TargetInteractable->InteractableData.InteractionDuration,      // Rate: 타이머 주기 (여기서는 상호작용 지속 시간)
					false                                                          // InbLoop: 반복 여부 (false: 한 번만 실행)
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
// 함수
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
					ETriggerEvent::Triggered,
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
		}
	}
}

