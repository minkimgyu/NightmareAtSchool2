// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InteractorInterface.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

// 첫 번째 InitializePickup 함수 (아이템 클래스를 인수로 받음)
void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
    if (ItemDataTable != nullptr && !DesiredItemID.IsNone())
    {
        // ItemDataTable에서 DesiredItemID를 사용하여 FItemData 구조체의 행을 찾습니다.
        const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

        // ItemReference 객체를 생성합니다. Outer는 이 APickup 액터로 설정하고 클래스는 BaseClass를 사용합니다.
        ItemReference = NewObject<UItemBase>(this, BaseClass);

        // 데이터 테이블에서 가져온 정보로 ItemReference의 속성을 설정합니다.
        ItemReference->ID = ItemData->ID;
        ItemReference->ItemType = ItemData->ItemType;
        ItemReference->Quantity = ItemData->Quantity;
        ItemReference->NumericData = ItemData->NumericData;
        ItemReference->TextData = ItemData->TextData;
        ItemReference->AssetData = ItemData->AssetData;

        bool canStackable = false;
        if (ItemData->NumericData.MaxStackSize > 1)
        {
            canStackable = true;
        }

        ItemReference->NumericData.bIsStackable = canStackable;
        // 수량을 체크하여 설정합니다. 0 이하일 경우 기본값 1로 설정합니다.
        InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

        // 픽업 메시 컴포넌트의 Static Mesh를 설정합니다.
        PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

        UpdateInteractableData();
    }
}

// 두 번째 InitializeDrop 함수 (아이템 인스턴스를 인수로 받음)
void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
    // ItemReference를 드롭된 아이템 인스턴스로 설정합니다.
    ItemReference = ItemToDrop;

    // 수량을 체크하여 설정합니다.
    InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

    // 아이템의 개별 무게를 가져와 ItemReference의 NumericData에 설정합니다.
    ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();

    // 픽업 메시를 설정합니다.
    PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

    // 상호작용 데이터를 업데이트합니다.
    UpdateInteractableData();
}


void APickup::UpdateInteractableData()
{
    // EInteractableType을 Pickup으로 설정합니다.
    InstanceInteractableData.InteractableType = EInteractableType::Pickup;

    // 상호작용 텍스트, 이름, 수량을 아이템 레퍼런스에서 가져와 설정합니다.
    InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
    InstanceInteractableData.Name = ItemReference->TextData.Name;
    InstanceInteractableData.Quantity = ItemReference->Quantity;

    // 최종 데이터를 InteractableData 변수에 저장합니다.
    InteractableData = InstanceInteractableData;
}

// InteractionInterface 함수들 (기본 구현 호출)

void APickup::BeginFocus()
{
    if (PickupMesh)
    {
        PickupMesh->SetRenderCustomDepth(true);
    }
}

void APickup::EndFocus()
{
    if (PickupMesh)
    {
        PickupMesh->SetRenderCustomDepth(false);
    }
}

void APickup::Interact(IInteractorInterface* Interactor)
{
    if(Interactor)
    {
        TakePickup(Interactor);
	}
}

// 픽업을 가져가는 커스텀 함수
void APickup::TakePickup(IInteractorInterface* Interactor)
{
    // 1. Kill Pending 상태 확인: 픽업 액터가 파괴 대기 중인지 확인합니다.
    if (!IsPendingKillPending())
    {
        // 2. 아이템 참조 확인: 픽업할 아이템에 대한 유효한 참조가 있는지 확인합니다.
        if (ItemReference)
        {
            // 3. 인벤토리 컴포넌트 가져오기: Taker 캐릭터로부터 인벤토리 컴포넌트를 가져옵니다.
            if (UInventoryComponent* PlayerInventory = Interactor->GetInventory())
            {
                // 4. 아이템 처리 시도: 인벤토리에 아이템을 추가하는 핸들러를 호출합니다.
                const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

                // 5. 아이템 추가 결과에 따른 처리
                switch (AddResult.OperationResult)
                {
                    // 아이템이 전혀 추가되지 않음 (예: 인벤토리 가득 참)
                    case EItemAddResult::IAR_NoItemAdded:
                        break;

                    case EItemAddResult::IAR_PartialAmountItemAdded:
                        // 일부만 추가됨 (예: 스택 제한)
                        // 남은 아이템 수량을 반영하여 픽업 액터의 상호작용 데이터를 업데이트합니다.
                        UpdateInteractableData();
                        // 플레이어 HUD 등의 위젯을 업데이트합니다.
                        //Taker->UpdateInteractionWidget();
                        break;

                    case EItemAddResult::IAR_AllItemAdded:
                        // 모든 아이템이 성공적으로 추가됨
                        // 픽업 액터를 월드에서 제거합니다.

                        // 임시 소리저장코드 -> 이벤트 바인드후 플레이어에게 들리게 조절로 수정 예정
                        UGameplayStatics::PlaySoundAtLocation(
                            this,               // 월드 컨텍스트
                            PickUpSound,     // 재생할 USoundBase 에셋
                            GetActorLocation(), // 재생될 위치
                            1.0f,               // 볼륨 배수
                            1.0f,               // 피치 배수
                            0.0f                // 시작 시간
                        );

                        Destroy();
                        break;
                }

                // 6. 로그 기록 (아이템 추가 결과 메시지)
                // *AddResult.ResultMessage.ToString()는 FText 또는 FString을 TEXT() 매크로가 기대하는 const TCHAR*로 변환하는 일반적인 방법입니다.
                UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
            }
            else
            {
                // 7. 인벤토리 컴포넌트가 null인 경우 로그 기록
                UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
            }
        }
        else
        {
            // 8. 픽업 아이템 참조가 null인 경우 로그 기록
            UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
        }
    }
}
#if WITH_EDITOR
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    // 변경된 속성이 DesiredItemID인지 확인합니다.
    if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
    {
        if (ItemDataTable)
        {
            // DesiredItemID로 데이터 테이블에서 아이템 데이터를 찾습니다.
            if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
            {
                // Static Mesh를 업데이트합니다.
                PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
            }
        }
    }
}
#endif
