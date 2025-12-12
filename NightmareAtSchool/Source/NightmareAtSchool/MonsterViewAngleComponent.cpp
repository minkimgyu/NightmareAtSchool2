// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterViewAngleComponent.h"
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

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerArray);

	// ...
	
}


// Called every frame
void UMonsterViewAngleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckMonsterLocation();
	CheckNearPlayer();
	CheckFindPlayer();

	// ...
}

void UMonsterViewAngleComponent::CheckMonsterLocation()
{
	MonsterLocation = GetOwner()->GetActorLocation();
}

float UMonsterViewAngleComponent::CalAngle(float _cos)
{
	float RadianValue = FMath::Acos(_cos);
	return FMath::RadiansToDegrees(RadianValue);
}

bool UMonsterViewAngleComponent::IsInAnglePlayer(AActor* _player)
{
	FVector MonsterPlayerAngle = _player->GetActorLocation() - MonsterLocation;
	MonsterPlayerAngle.Z = 0.0f;
	MonsterPlayerAngle.Normalize(); // 크기 1로 조정

	//UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *MonsterPlayerAngle.ToString());

	FVector MonsterForwardVector = GetOwner()->GetActorForwardVector().RotateAngleAxis(90.f, FVector(0, 0, 1));

	float HalfViewAngle = CalAngle(FVector::DotProduct(MonsterPlayerAngle, MonsterForwardVector));

	if (HalfViewAngle <= ViewAngle/2)
	{
		return true;
	}

	return false;
}

void UMonsterViewAngleComponent::CheckNearPlayer()
{
	for (auto Player : PlayerArray)
	{
		if (!Player) // 플레이어가 없으면 생략
		{
			continue;
		}

		float Dist = FVector::Dist(MonsterLocation, Player->GetActorLocation());

		if (Dist <= ViewDistance)
		{
			if (!NearPlayerArray.Find(Player))
			{
				NearPlayerArray.Add(Player);
			}
		}
		else
		{
			if (NearPlayerArray.Find(Player))
			{
				NearPlayerArray.Remove(Player);
			}
		}
	}
}
bool UMonsterViewAngleComponent::IsObstacle(AActor* _player)
{
	FHitResult HitResult;

	FVector Start = GetOwner()->GetActorLocation();
	TArray<FVector> PlayerCheckActor;;

	TArray<AActor*> PlayerHMF;
	_player->GetAttachedActors(PlayerHMF);

	for (auto Child : PlayerHMF)
	{
		PlayerCheckActor.Add(Child->GetActorLocation());
	}
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner()); // 자기 자신 제외
	Params.AddIgnoredActor(GetOwner()->GetAttachParentActor()); // 눈의 주인인 몬스터 제외

	for (FVector End : PlayerCheckActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *End.ToString());
		bool isHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

		if (isHit && HitResult.GetActor()->ActorHasTag("PlayerCheckActor"))
		{
			DrawDebugBox(
				GetWorld(),
				HitResult.Location,
				FVector(5.0f, 5.0f, 5.0f),
				FColor::Blue,
				false,
				0.5f,
				0,
				3.0f
			);
			return false;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Monster don't find player"));
	return true;
}

AActor* UMonsterViewAngleComponent::CheckFindPlayer() // 찾아가야할 플레이어 함수
{
	for (auto Player : NearPlayerArray)
	{
		if (!Player)
		{
			continue;
		}

		bool IsViewPlayer = IsInAnglePlayer(Player);

		if (IsViewPlayer)
		{
			if (!IsObstacle(Player))
			{
				FString ActorName = Player->GetName();
				UE_LOG(LogTemp, Warning, TEXT("monster find player"), *ActorName);
				return Player; // 저장된 순서로 발견한 플레이어 찾아가게함
			}
		}

	}
	return nullptr; // 숨어있으면 빈 주소 반환
}

FVector UMonsterViewAngleComponent::GetMonsterLocation() const
{
	return MonsterLocation;
}
