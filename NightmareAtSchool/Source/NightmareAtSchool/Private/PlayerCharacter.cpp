// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UserInterface/MainHUD.h"

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
	HUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
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

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	TargetInteractable->BeginFocus();
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

