// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

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

void APickup::Interact(APlayerCharacter* PlayerCharacter)
{
    if(PlayerCharacter)
    {
        TakePickup(PlayerCharacter);
	}
}

// 픽업을 가져가는 커스텀 함수
void APickup::TakePickup(const APlayerCharacter* Taker)
{
    if (!IsPendingKillPending())
    {
        if (ItemReference)
        {

        }
    }
}

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

