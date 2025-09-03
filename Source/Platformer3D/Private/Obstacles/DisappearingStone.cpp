// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/DisappearingStone.h"
#include "Components/BoxComponent.h"

ADisappearingStone::ADisappearingStone()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
		Mesh->SetupAttachment(GetRootComponent());
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
		BoxComp->SetupAttachment(Mesh);

}


void ADisappearingStone::BeginPlay()
{
	Super::BeginPlay();
	
}


void ADisappearingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

