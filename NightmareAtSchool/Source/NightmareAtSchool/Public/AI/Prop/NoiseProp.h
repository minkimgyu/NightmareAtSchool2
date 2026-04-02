// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoiseProp.generated.h"

// 클래스 전방 선언
class USphereComponent;
class UStaticMeshComponent; // 클래스 전방 선언 추가

UCLASS()
class NIGHTMAREATSCHOOL_API ANoiseProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoiseProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // 외형을 담당하는 스태틱 메쉬 컴포넌트 (유리조각 등)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // 트리거 콜라이더
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    class UAudioComponent* AudioComp;

    // 2. 소음 발생 함수 (오버랩 이벤트 바인딩용)
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // 소음 세기 (0.0 ~ 1.0 권장)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Noise")
    float NoiseLoudness = 1.0f;

    // 소음 전달 최대 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Noise")
    float NoiseMaxRange = 1500.0f;


    // --- 디버그 설정 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Debug")
    bool bShowDebugCircle = true; // 디버그 켜기/끄기

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Debug")
    FColor DebugCircleColor = FColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Debug")
    float DebugCircleLifeTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Debug")
    float DebugThickness = 2.0f;
};
