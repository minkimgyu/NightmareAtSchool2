// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/ElectricBox.h"

#include "InteractionInterface.h" // APlayerCharacter.h에서 사용되던 인터페이스

#include "Components/StaticMeshComponent.h"
#include "Components/InventoryComponent.h"
#include "InteractorInterface.h"

#include "Kismet/GameplayStatics.h" // PlaySoundAtLocation 사용을 위해 필요

// Sets default values
AElectricBox::AElectricBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 설정
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(Root);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(Root);

	// 게임 시작 시 보이지 않게 설정 (기본값)
	BoxMesh->SetHiddenInGame(true);

	bIsPlanted = false;

	UE_LOG(LogTemp, Log, TEXT("Name1: %s Desc1: %s"),
		*InteractableData.Name.ToString(),
		*InteractableData.Action.ToString());
}

// Called when the game starts or when spawned
void AElectricBox::BeginPlay()
{
	Super::BeginPlay();

	// 상호작용 데이터 초기화
	InteractableData.InteractionDuration = 0.0f; // 즉시 상호작용
	InteractableData.Action = FText::FromString("Install");
	InteractableData.Name = FText::FromString("the electric box");

	UE_LOG(LogTemp, Log, TEXT("Name: %s Desc: %s"),
		*InteractableData.Name.ToString(),
		*InteractableData.Action.ToString());
}

// Called every frame
void AElectricBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AElectricBox::Interact(IInteractorInterface* Interactor)
{
	// 문 상호작용을 처리하는 핵심 로직 호출
	HandleInteraction(Interactor);
}

void AElectricBox::BeginFocus()
{
	// 메시 아웃라인이나 강조 효과를 처리할 수 있습니다.
	WallMesh->SetRenderCustomDepth(true);
}

void AElectricBox::EndFocus()
{
	// 강조 효과를 제거합니다.
	WallMesh->SetRenderCustomDepth(false);
}

// ADoorBase의 HandleInteraction 기본 구현 (빈 함수 또는 간단한 로직)
void AElectricBox::HandleInteraction(IInteractorInterface* Interactor)
{
	//Super::HandleInteraction(PlayerCharacter);

	if (!Interactor) return;
	if (bIsPlanted) return;
	if (CanInstallElectricBox(Interactor) == false) return;

	if (bIsPlanted == false)
	{
		// 아이템 제거
		UInventoryComponent* Inventory = Interactor->GetInventory();
		Inventory->RemoveItemByID(RequiredItemID, RequiredItemAmount);

		// 닫는 로직 (C++ 회전 시작)
		bIsPlanted = true;

		// 등록된 모든 리스너에게 이벤트 전송 (블루프린트의 Call)
		OnElectricBoxInstalled.Broadcast();


		if (BoxInstallSound)
		{
			FVector SoundLocation = GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(this, BoxInstallSound, SoundLocation);
		}

		// 렌더링을 활성화하여 보이게 만듦
		BoxMesh->SetHiddenInGame(false);
	}
	
	// 로그를 찍어서 함수가 실행되는지 확인하세요!
	UE_LOG(LogTemp, Warning, TEXT("Box Interacted! bIsPlanted: %s"), bIsPlanted ? TEXT("True") : TEXT("False"));

	Interactor->UpdateInteractionWidget(&InteractableData);
}

bool AElectricBox::CanInstallElectricBox(IInteractorInterface* Interactor) const
{
	if (!Interactor) return false;

	// 1. 플레이어의 인벤토리 컴포넌트 가져오기
	UInventoryComponent* Inventory = Interactor->GetInventory();

	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("AElectricBox: PlayerCharacter에 UInventoryComponent가 없습니다."));
		return false;
	}

	// 2. 요구 사항 아이템 ID가 유효한지 확인
	if (RequiredItemID == NAME_None || RequiredItemAmount <= 0)
	{
		// 요구 사항이 없으면 항상 열 수 있다고 가정
		return true;
	}

	// 3. 인벤토리에 필요한 수량이 있는지 확인 (이전 단계에서 구현한 함수 사용)
	const int32 CurrentAmount = Inventory->GetItemAmountByID(RequiredItemID);

	UE_LOG(LogTemp, Warning, TEXT("ID: %s, Required: %d, Inventory: %d"),
		*RequiredItemID.ToString(), RequiredItemAmount, CurrentAmount);

	return CurrentAmount >= RequiredItemAmount;
}