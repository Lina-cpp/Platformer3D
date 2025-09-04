// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/DisappearingStone.h"

#include "Characters/CharacterBase.h"
#include "Components/BoxComponent.h"

ADisappearingStone::ADisappearingStone()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
		Mesh->SetupAttachment(GetRootComponent());
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
		BoxComp->SetupAttachment(Mesh);
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADisappearingStone::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADisappearingStone::BoxCompEndOverlap);

	UMaterialInstanceDynamic* DynMaterial = Mesh->CreateDynamicMaterialInstance(0, Mesh->GetMaterial(0));
}


void ADisappearingStone::BeginPlay()
{
	Super::BeginPlay();

}

void ADisappearingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/* Box Overlaps */
	//BeginOverlap
void ADisappearingStone::BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check if player can stand on rock (is visible and has collision)
	if (!bCanStandOnRock) return;
	//Cast to check if actor on stone is player
	if (ACharacterBase* Char = Cast<ACharacterBase>(OtherActor))
	{
		GetWorldTimerManager().ClearTimer(StandOnTimer); //Clear Timer, just in case
		//Call Timer for 2 sec and then call its function
		GetWorldTimerManager().SetTimer(StandOnTimer,this, &ADisappearingStone::PlayerOnStone, StandOnTime, false);
		//Call function from BP
		BeginOverlap_Implementation();
	}
}


/* Timers */
void ADisappearingStone::PlayerOnStone()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bCanStandOnRock = false;
}

void ADisappearingStone::BeginOverlap_Implementation()
{
	
}



	//End Overlap
void ADisappearingStone::BoxCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (ACharacterBase* Char = Cast<ACharacterBase>(OtherActor))
	{
		GetWorldTimerManager().ClearTimer(StandOnTimer);
		GetWorldTimerManager().SetTimer(DisappearTimer, this, &ADisappearingStone::StoneDisappeared, ReAppearTime, false);
	}
}

void ADisappearingStone::StoneDisappeared()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bCanStandOnRock = true;
	EndOverlap_Implementation();
}


void ADisappearingStone::EndOverlap_Implementation()
{
	
}


