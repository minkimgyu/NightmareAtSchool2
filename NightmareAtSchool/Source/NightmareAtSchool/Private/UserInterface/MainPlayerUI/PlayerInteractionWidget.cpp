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

void UPlayerInteractionWidget::BU_RunClicked()
{
    isRun = !isRun;

    if (isRun)
    {
        PlayerChar->StartSprint();
        IM_Run->SetRenderOpacity(1.0);
    }
    else
    {
        PlayerChar->StopSprint();
        IM_Run->SetRenderOpacity(0.7);
    }
}

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
        BU_Run->OnClicked.AddDynamic(this, &UPlayerInteractionWidget::BU_RunClicked);

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
