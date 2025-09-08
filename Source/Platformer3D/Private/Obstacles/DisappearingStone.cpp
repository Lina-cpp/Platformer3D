// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/DisappearingStone.h"

#include "Characters/CharacterBase.h"
#include "Components/BoxComponent.h"

ADisappearingStone::ADisappearingStone()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
		Mesh->SetupAttachment(GetRootComponent());
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
		BoxComp->SetupAttachment(Mesh);
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADisappearingStone::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADisappearingStone::BoxCompEndOverlap);

	
}


void ADisappearingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpacityTimeline.TickTimeline(DeltaTime);
}


void ADisappearingStone::BeginPlay()
{
	Super::BeginPlay();

/* Dynamic Material setup */
	if (Mesh && Mesh->GetMaterial(0))
	{
		DynMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
		Mesh->SetMaterial(0, DynMaterial);		
	}

/* Curve and Timeline Setup */
	if (OpacityCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress"));

		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindUFunction(this, FName("HandleTimelineFinished"));

		OpacityTimeline.AddInterpFloat(OpacityCurve, ProgressFunction);
		OpacityTimeline.SetTimelineFinishedFunc(FinishedFunction);

		OpacityTimeline.SetLooping(false);
	}
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

		//Call Timeline to Change Opacity
		OpacityTimeline.Stop();
		OpacityTimeline.SetPlayRate( 1 / StandOnTime ); // To Match Opacity change to disappearing time
		OpacityTimeline.ReverseFromEnd();
	}
}

void ADisappearingStone::PlayerOnStone()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bCanStandOnRock = false;
}



	//End Overlap
void ADisappearingStone::BoxCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (ACharacterBase* Char = Cast<ACharacterBase>(OtherActor))
	{
		GetWorldTimerManager().ClearTimer(StandOnTimer);
		GetWorldTimerManager().SetTimer(DisappearTimer, this, &ADisappearingStone::StoneDisappeared, ReAppearTime, false);
		//if player left stone before it disappeared restore full opacity
		if (bCanStandOnRock)
		{
			OpacityTimeline.Stop();
			GetWorldTimerManager().ClearTimer(DisappearTimer);
			DynMaterial->SetScalarParameterValue("Opacity", 1.0f);
		}
	}
}

void ADisappearingStone::StoneDisappeared()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bCanStandOnRock = true;
	OpacityTimeline.Stop();
	OpacityTimeline.SetPlayRate(1 / ReAppearTime);
	OpacityTimeline.PlayFromStart();
}




/* Dynamic Material & Timeline */
void ADisappearingStone::HandleTimelineProgress(float Value)
{
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue("Opacity", Value);
	}
}

void ADisappearingStone::HandleTimelineFinished()
{
	
}