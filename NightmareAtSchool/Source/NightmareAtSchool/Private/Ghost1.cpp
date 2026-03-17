// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost1.h"
#include "BlackFigureDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGhost1::AGhost1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AGhost1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhost1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AGhost1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

