// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject/PushPullDoor.h"
#include "ExitDoor.generated.h"

// 델리게이트 선언 (매개변수가 없는 형태)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpenedSignature);

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API AExitDoor : public APushPullDoor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExitDoor();

	UFUNCTION(BlueprintCallable)
	void UnlockDoor();

    // 블루프린트에서 호출하거나 바인딩할 수 있도록 노출
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDoorOpenedSignature OnDoorOpened;

protected:
    // ----------------------------------------------------------------------
    // 잠금 요구 사항
    // ----------------------------------------------------------------------

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** 이 문이 영구적으로 잠금 해제되었는지 여부. 한번 해제되면 다시 잠기지 않습니다. */
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Lock Door | State")
    bool bIsPermanentlyUnlocked = false; // ⬅️ 추가

    // ----------------------------------------------------------------------
    // 핵심 로직
    // ----------------------------------------------------------------------

    /** 부모 클래스의 상호 작용 로직을 오버라이드하여 잠금 해제 로직을 추가합니다. */
    virtual void HandleInteraction(APlayerCharacter* PlayerCharacter) override;
};
