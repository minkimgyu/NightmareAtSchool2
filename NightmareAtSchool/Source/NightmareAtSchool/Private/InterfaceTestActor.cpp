// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceTestActor.h"
#include "InteractorInterface.h"


// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableData;
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

/**
 * 상호작용 시작 인터페이스 함수 오버라이드
 */
void AInterfaceTestActor::BeginInteract()
{
	// 인터페이스 테스트 액터에서 BeginInteract 오버라이드가 호출되었음을 경고 레벨로 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

/**
 * 상호작용 종료 인터페이스 함수 오버라이드
 */
void AInterfaceTestActor::EndInteract()
{
	// 인터페이스 테스트 액터에서 EndInteract 오버라이드가 호출되었음을 경고 레벨로 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

/**
 * 실제 상호작용 처리 인터페이스 함수 오버라이드
 */
void AInterfaceTestActor::Interact(IInteractorInterface* Interactor)
{
	// 인터페이스 테스트 액터에서 Interact 오버라이드가 호출되었음을 경고 레벨로 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));
}