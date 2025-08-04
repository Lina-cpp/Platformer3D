// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/RotatingObstacle.h"

#include "GameFramework/RotatingMovementComponent.h"


ARotatingObstacle::ARotatingObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
		RootComponent = SceneRoot;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
		StaticMesh->SetupAttachment(SceneRoot);

	
	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");

}

void ARotatingObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARotatingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

