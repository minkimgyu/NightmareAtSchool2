// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionInterface.h"
#include "PlayerCharacter.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

void IInteractionInterface::BeginFocus()
{
}

void IInteractionInterface::EndFocus()
{
}

/**
 * 상호작용 시작 인터페이스 함수 오버라이드
 */
void IInteractionInterface::BeginInteract()
{
}

/**
 * 상호작용 종료 인터페이스 함수 오버라이드
 */
void IInteractionInterface::EndInteract()
{
}

/**
 * 실제 상호작용 처리 인터페이스 함수 오버라이드
 */
void IInteractionInterface::Interact(APlayerCharacter* PlayerCharacter)
{
}