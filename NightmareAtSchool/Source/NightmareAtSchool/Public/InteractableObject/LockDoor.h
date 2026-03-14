// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject/PushPullDoor.h"
#include "LockDoor.generated.h"

UCLASS()
class NIGHTMAREATSCHOOL_API ALockDoor : public APushPullDoor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockDoor();

protected:
    // ----------------------------------------------------------------------
    // 잠금 요구 사항
    // ----------------------------------------------------------------------

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** 문을 열기 위해 인벤토리에 필요한 아이템의 ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Door | Requirements")
    FName RequiredItemID;

    /** 문을 열기 위해 인벤토리에 필요한 아이템의 최소 수량 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Door | Requirements", meta = (ClampMin = "1"))
    int32 RequiredItemAmount = 1;

    /** 이 문이 영구적으로 잠금 해제되었는지 여부. 한번 해제되면 다시 잠기지 않습니다. */
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Lock Door | State")
    bool bIsPermanentlyUnlocked = false; // ⬅️ 추가

    // ----------------------------------------------------------------------
    // 핵심 로직
    // ----------------------------------------------------------------------

    /** 부모 클래스의 상호 작용 로직을 오버라이드하여 잠금 해제 로직을 추가합니다. */
    virtual void HandleInteraction(APlayerCharacter* PlayerCharacter) override;

    /**
     * @brief 플레이어의 인벤토리를 확인하여 문을 열 수 있는 자격이 있는지 검사합니다.
     * @param PlayerCharacter 검사할 플레이어입니다.
     * @return 요구 사항이 충족되면 true, 아니면 false를 반환합니다.
     */
    bool CanUnlockDoor(APlayerCharacter* PlayerCharacter) const;

};
