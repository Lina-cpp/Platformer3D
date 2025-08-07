// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/RotatingObstacle.h"
#include "Characters/CharacterBase.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/RotatingMovementComponent.h"


ARotatingObstacle::ARotatingObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
		RootComponent = SceneRoot;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
		StaticMesh->SetupAttachment(SceneRoot);
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
		CapsuleComp->SetupAttachment(StaticMesh);
		CapsuleComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
		CapsuleComp->SetCapsuleHalfHeight(520.f);
		CapsuleComp->SetCapsuleRadius(62.f);

	
	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");

}

void ARotatingObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARotatingObstacle::CallOnHitInterface(AActor* HitActor)
{
	//if hit actor is null - leave function
	if (!HitActor) return;

	//Check if Hit actor has interface
	if (HitActor->Implements<UHitInterface>())
	{
		IHitInterface::Execute_OnHit(HitActor);
	}
}

void ARotatingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

