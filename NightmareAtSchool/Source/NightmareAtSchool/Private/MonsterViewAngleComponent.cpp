// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterViewAngleComponent.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
//#include "AIController.h"
//#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UMonsterViewAngleComponent::UMonsterViewAngleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsterViewAngleComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> PlayerArray; // 게임에 있는 모든 플레이어를 저장할 변수
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerArray); // 플레이어 태그가 달린 모든 것을 받아오고

	for (AActor* PActor : PlayerArray)
	{
		FPlayerScanGruop PlayerScanGroup;

		PlayerScanGroup.PlayerActor = PActor;

		TArray<USceneComponent*> SceneComps; // 조사할 부위들
		PActor->GetComponents<USceneComponent>(SceneComps); // 받아오고

		PlayerScanGroup.ScanPoints = SceneComps;
		PlayerScanArray.Add(PlayerScanGroup);
	}
	
	//타이머 설정해서 함수 지연실행으로 약간의 최적화
	GetWorld()->GetTimerManager().SetTimer(ViewCheckTimerHandle, this, &UMonsterViewAngleComponent::MonsterViewExe, 0.5f, true);
	//GetAIController();

	// ...
	
}


// Called every frame
void UMonsterViewAngleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	MonsterViewExe();
	// ...
}

void UMonsterViewAngleComponent::MonsterViewExe()
{
	CheckMonsterLocation();
	CheckNearPlayer();
	FoundPlayer = CheckFindTarget();
	HandleTargetChange();
	//SaveBlackboard(); 
	DrawViewAngle();
}

void UMonsterViewAngleComponent::HandleTargetChange()
{
	if (FoundPlayer != NewPlayer)
	{
		//이벤트
		OnTarget.Broadcast(FoundPlayer);
		NewPlayer = FoundPlayer;
	}
}

//몬스터 위치 체크
void UMonsterViewAngleComponent::CheckMonsterLocation()
{
	MonsterLocation = GetOwner()->GetActorLocation();
}

//각도 계산
float UMonsterViewAngleComponent::CalAngle(float _cos) const
{
	float RadianValue = FMath::Acos(_cos);
	return FMath::RadiansToDegrees(RadianValue);
}

//플레이어가 가까이 있는가
void UMonsterViewAngleComponent::CheckNearPlayer()
{
	for (const FPlayerScanGruop& PlayerScan : PlayerScanArray)
	{
		AActor* Player = PlayerScan.PlayerActor;
		if (!Player) // 플레이어가 없으면 생략
		{
			continue;
		}

		float Dist = FVector::Dist(MonsterLocation, Player->GetActorLocation());

		if (Dist <= ViewDistance)
		{
			NearPlayerArray.Add(PlayerScan);
		}
		else
		{
			NearPlayerArray.Remove(PlayerScan);
		}
	}
}

//플레이어가 각도 안에 들어가 있는가
bool UMonsterViewAngleComponent::IsInAngleTarget(AActor* _player) const
{
	FVector MonsterPlayerAngle = _player->GetActorLocation() - MonsterLocation;
	MonsterPlayerAngle.Z = 0.0f;
	MonsterPlayerAngle.Normalize(); // 크기 1로 조정

	//UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *MonsterPlayerAngle.ToString());

	//FVector MonsterForwardVector = GetOwner()->GetActorForwardVector().RotateAngleAxis(90.f, FVector(0, 0, 1));
	FVector MonsterForwardVector = GetOwner()->GetActorForwardVector();

	float HalfViewAngle = CalAngle(FVector::DotProduct(MonsterPlayerAngle, MonsterForwardVector));

	if (HalfViewAngle <= ViewAngle/2)
	{
		return true;
	}

	return false;
}

//해당 몬스터가 몬스터 인지 각도에 들어온 플레이어가 실제로 눈에 보이는가
bool UMonsterViewAngleComponent::IsObstacle(FPlayerScanGruop _PlayerScanGroup) const
{
	FHitResult HitResult;

	FVector Start = GetOwner()->GetActorLocation(); // 이 컴포넌트를 지니고 있는 귀신의 위치가 시작점
	//TArray<FVector> PlayerCheckActor;

	//TArray<AActor*> PlayerHMF;
	//_player->GetAttachedActors(PlayerHMF);

	//for (auto Child : PlayerHMF)
	//{
	//	PlayerCheckActor.Add(Child->GetActorLocation()); // 
	//}
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner()); // 자기 자신 제외
	//Params.AddIgnoredActor(GetOwner()->GetAttachParentActor()); // 눈의 주인인 몬스터 제외

	for (USceneComponent* ScanPoint : _PlayerScanGroup.ScanPoints)
	{
		if (!ScanPoint) continue;

		FVector End = ScanPoint->GetComponentLocation(); // 플레이어의 손이나 발, 몸통 부위

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params); // 레이저쏴서 맞은게 있냐

		if (bIsHit) // 있다면
		{
			AActor* HitActor = HitResult.GetActor(); 

			if (HitActor && 
				(HitActor == _PlayerScanGroup.PlayerActor || HitActor->ActorHasTag(TEXT("PlayerCheckActor"))))
			{
				UE_LOG(LogTemp, Warning, TEXT("player success part : %s"), *ScanPoint->GetName());

				DrawDebugBox(
					GetWorld(),
					HitResult.Location,
					FVector(5.0f),
					FColor::Blue,
					false,
					0.5f,
					0,
					3.0f
				);

				return true;;
			}

			return false;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Monster don't find player"));
	return false;
}

AActor* UMonsterViewAngleComponent::CheckFindTarget() const
{
	for (auto PlayerScan : NearPlayerArray)
	{
		if (!PlayerScan.PlayerActor)
		{
			continue;
		}
		AActor* Player = PlayerScan.PlayerActor;
		bool IsViewPlayer = IsInAngleTarget(Player);

		if (IsViewPlayer)
		{
			if (IsObstacle(PlayerScan))
			{
				FString ActorName = Player->GetName();
				//UE_LOG(LogTemp, Warning, TEXT("monster find player"), *ActorName);
				return Player; // 저장된 순서로 발견한 플레이어 찾아가게함
			}
		}

	}
	return nullptr; // 숨어있으면 빈 주소 반환
}

//찾아가야할 플레이어 함수
//
//void UMonsterViewAngleComponent::SaveBlackboard()
//{
//	if (AIController)
//	{
//		Blackboard = AIController->GetBlackboardComponent();
//
//		if (FoundPlayer)
//		{
//			if (Blackboard)
//			{
//				UE_LOG(LogTemp, Warning, TEXT("SUCCESS BLACKBOARD"));
//				Blackboard->SetValueAsObject(TEXT("Player"), FoundPlayer);
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Clear"));
//			Blackboard->ClearValue(TEXT("Player"));
//		}
//	}
//}
//
//void UMonsterViewAngleComponent::GetAIController()
//{
//	UE_LOG(LogTemp, Warning, TEXT("GetAIController"));
//	OwnerPawn = Cast<APawn>(GetOwner());
//	if (!OwnerPawn)
//	{
//		return;
//	}
//
//	AIController = Cast<AAIController>(OwnerPawn->GetController());
//	if (!AIController)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("DISCOVER AICONTROLLER"));
//		return;
//	}
//}
void UMonsterViewAngleComponent::DrawViewAngle()
{
	//FVector MonsterFoward = GetOwner()->GetActorForwardVector().RotateAngleAxis(90.f, FVector(0, 0, 1));
	FVector MonsterFoward = GetOwner()->GetActorForwardVector();
	FVector LeftDir = MonsterFoward.RotateAngleAxis(-ViewAngle/2, FVector(0, 0, 1));
	FVector RightDir = MonsterFoward.RotateAngleAxis(ViewAngle / 2, FVector(0, 0, 1));

	DrawDebugLine(
		GetWorld(),
		MonsterLocation,
		MonsterLocation + MonsterFoward * ViewDistance,
		FColor::Red,
		false,
		0.f,
		SDPG_Foreground,
		2.f
	);

	DrawDebugLine(
		GetWorld(),
		MonsterLocation,
		MonsterLocation + LeftDir * ViewDistance,
		FColor::Red,
		false,
		0.f,
		SDPG_Foreground,
		2.f
	);

	// 오른쪽 시야선
	DrawDebugLine(
		GetWorld(),
		MonsterLocation,
		MonsterLocation + RightDir * ViewDistance,
		FColor::Red,
		false,
		0.f,
		SDPG_Foreground,
		2.f
	);
}

FVector UMonsterViewAngleComponent::GetMonsterLocation() const
{
	return MonsterLocation;
}
