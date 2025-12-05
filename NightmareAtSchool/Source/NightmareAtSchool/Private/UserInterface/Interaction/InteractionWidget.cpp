// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Interaction/InteractionWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InteractionProgressBar->PercentDelegate.BindUFunction(
        this,
        FName("UpdateInteractionProgress")
    );
}

void UInteractionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    KeyPressText->SetText(FText::FromString("Press"));
    CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
    switch (InteractableData->InteractableType)
    {
    case EInteractableType::Pickup:
        // KeyPressText->SetText(FText::FromString("Press"));
        // FText::FromString은 보통 NSLOCTEXT 매크로로 대체되므로, 이미지에 보이는 텍스트로 대체했습니다.
        KeyPressText->SetText(FText::FromString(TEXT("Press")));

        InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

        if (InteractableData->Quantity < 2)
        {
            QuantityText->SetVisibility(ESlateVisibility::Collapsed);
        }
        else // InteractableData->Quantity >= 2
        {
            QuantityText->SetText(FText::Format(
                NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),
                InteractableData->Quantity
            ));

            QuantityText->SetVisibility(ESlateVisibility::Visible);
        }

        break;

    case EInteractableType::NonPlayerCharacter:
        break;

    case EInteractableType::Device:
        break;

    case EInteractableType::Toggle:
        break;

    case EInteractableType::Container:
        break;

    default:
        break;
    }

    ActionText->SetText(InteractableData->Action);
    NameText->SetText(InteractableData->Name);
}

float UInteractionWidget::UpdateInteractionProgress()
{
    return 0.f;
}