// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"


class APlayerCharacter;

/**
 * 상호작용 가능한 객체의 유형을 정의합니다.
 */
UENUM(BlueprintType)
enum class EInteractableType : uint8
{
    /** 아이템 줍기 (예: 무기, 소비품) */
    Pickup UMETA(DisplayName = "Pickup"),

    /** 플레이어가 아닌 캐릭터 (예: NPC, 상인) */
    NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),

    /** 조작 가능한 장치 (예: 문, 스위치) */
    Device UMETA(DisplayName = "Device"),

    /** 켜거나 끌 수 있는 요소 (예: 조명, 간단한 스위치) */
    Toggle UMETA(DisplayName = "Toggle"),

    /** 아이템을 저장할 수 있는 컨테이너 (예: 상자, 보관함) */
    Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_BODY()

    FInteractableData() :
        InteractableType(EInteractableType::Pickup), // EInteractableType::Pickup으로 초기화
        Name(FText::GetEmpty()),                  // FText를 빈 값으로 초기화
        Action(FText::GetEmpty()),                // FText를 빈 값으로 초기화
        Quantity(0),                               // 수량/개수를 0으로 초기화 (새로 추가된 멤버)
        InteractionDuration(0.0f)                 // 상호작용 지속 시간을 0.0f로 초기화 (새로 추가된 멤버)
    {

    };
        
    /** 상호작용 가능한 객체의 유형입니다. */
    UPROPERTY(EditInstanceOnly)
    EInteractableType InteractableType;

    /** 상호작용 가능한 객체의 이름 (예: "낡은 열쇠", "상인") */
    UPROPERTY(EditInstanceOnly)
    FText Name;

    /** 플레이어에게 표시될 상호작용 액션 (예: "줍기", "대화하기", "열기") */
    UPROPERTY(EditInstanceOnly)
    FText Action;

    // 픽업 시 사용
    UPROPERTY(EditInstanceOnly)
    int8 Quantity;

    // 상호 작용 기간
    UPROPERTY(EditInstanceOnly)
    float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NIGHTMAREATSCHOOL_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();

	virtual void BeginInteract();
	virtual void EndInteract();

	virtual void Interact(APlayerCharacter* PlayerCharacter); // [00:04:21]

	FInteractableData InteractableData;
};
