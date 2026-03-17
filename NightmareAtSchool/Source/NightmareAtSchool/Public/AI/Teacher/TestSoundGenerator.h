// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestSoundGenerator.generated.h"

UCLASS()
class NIGHTMAREATSCHOOL_API ATestSoundGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestSoundGenerator();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 소리를 발생시키는 함수
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "AI|Sound")
    void GenerateNoise();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    float SoundRange = 1000.f; // 소리가 들리는 범위

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    float Loudness = 1.0f; // 소리 크기

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    bool bShowDebugSphere = true; // 범위 시각화 여부

};
