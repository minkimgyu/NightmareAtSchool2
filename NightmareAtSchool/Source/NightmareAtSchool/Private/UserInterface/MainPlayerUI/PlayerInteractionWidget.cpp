// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainPlayerUI/PlayerInteractionWidget.h"

#include "PlayerCharacter.h"

#include "Components/InteractionComponent.h"

void UPlayerInteractionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 생성 시점에 플레이어 캐릭터를 찾아 변수에 저장
    PlayerChar = Cast<APlayerCharacter>(GetOwningPlayerPawn());
    if (!PlayerChar)
    {
        UE_LOG(LogTemp, Error, TEXT("what the hell"));
        return;
    }

    InteractionComp = PlayerChar->FindComponentByClass<UInteractionComponent>();
    if (!InteractionComp)
    {
        return;
    }

    InitUIOpacity();
}

void UPlayerInteractionWidget::BU_PickUpClicked()
{
    InteractionComp->Interact();
}

void UPlayerInteractionWidget::BU_SitDownClicked()
{
    isSit = !isSit;

    PlayerChar->ToggleCrouch();

    if (isSit)
    {
        IM_SitDown->SetRenderOpacity(1.0);
    }
    else
    {
        IM_SitDown->SetRenderOpacity(0.7);
    }
}

void UPlayerInteractionWidget::BU_RunPressed()
{
    if (PlayerChar)
    {
        PlayerChar->StartSprint();
        //IM_Run->SetRenderOpacity(1.0);
        // 투명도는 아래 NativeTick에서 실시간으로 처리하는 것이 더 정확합니다.
    }
}

void UPlayerInteractionWidget::BU_RunReleased()
{
    if (PlayerChar)
    {
        PlayerChar->StopSprint();
        //IM_Run->SetRenderOpacity(0.7);
    }
}

// ⭐ 가장 중요한 부분: 캐릭터 상태와 UI 투명도 동기화
void UPlayerInteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (PlayerChar && IM_Run)
    {
        // 캐릭터의 실제 ActionState를 확인합니다.
        // (주의: PlayerCharacter.h에서 PlayerActionState가 protected라면 public getter를 만들거나 접근 가능해야 합니다)

        // 여기서는 예시로 캐릭터가 실제로 달리고 있는지 확인하여 투명도를 조절합니다.
        if (PlayerChar->GetPlayerActionState() == EPlayerActionState::Sprint)
        {
            IM_Run->SetRenderOpacity(1.0f);
        }
        else
        {
            IM_Run->SetRenderOpacity(0.7f);
        }
    }
}

//void UPlayerInteractionWidget::BU_RunClicked()
//{
//    isRun = !isRun;
//
//    if (isRun)
//    {
//        PlayerChar->StartSprint();
//        IM_Run->SetRenderOpacity(1.0);
//    }
//    else
//    {
//        PlayerChar->StopSprint();
//        IM_Run->SetRenderOpacity(0.7);
//    }
//}

void UPlayerInteractionWidget::BU_LightClicked()
{
    isOnFlashLight = !isOnFlashLight;

    PlayerChar->ToggleFlashlight();
    if (isOnFlashLight)
    {
        IM_Light->SetRenderOpacity(1.0);
    }
    else
    {
        IM_Light->SetRenderOpacity(0.7);
    }
}

void UPlayerInteractionWidget::InitUIOpacity()
{
    //버튼 클릭 연동
    if (BU_PickUp)
        BU_PickUp->OnClicked.AddDynamic(this, &UPlayerInteractionWidget::BU_PickUpClicked);

    if (BU_SitDown)
        BU_SitDown->OnClicked.AddDynamic(this, &UPlayerInteractionWidget::BU_SitDownClicked);

    if (BU_Run)
    {
        //BU_Run->OnClicked.AddDynamic(this, &UPlayerInteractionWidget::BU_RunClicked);

        // OnClicked 대신 Pressed와 Released를 사용합니다.
        BU_Run->OnPressed.AddDynamic(this, &UPlayerInteractionWidget::BU_RunPressed);
        BU_Run->OnReleased.AddDynamic(this, &UPlayerInteractionWidget::BU_RunReleased);
    }

    if (BU_Light)
        BU_Light->OnClicked.AddDynamic(this, &UPlayerInteractionWidget::BU_LightClicked);

    //투명도 초기 조절
    if (IM_SitDown)
    {
        IM_SitDown->SetRenderOpacity(0.7);
    }

    if (IM_Run)
    {
        IM_Run->SetRenderOpacity(0.7);
    }

    if (IM_Light)
    {
        IM_Light->SetRenderOpacity(0.7);
    }
}
