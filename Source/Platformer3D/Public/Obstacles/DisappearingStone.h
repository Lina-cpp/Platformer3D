// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "DisappearingStone.generated.h"

class UBoxComponent;
class ACharacterBase;


UCLASS()
class PLATFORMER3D_API ADisappearingStone : public AActor
{
	GENERATED_BODY(	)
	
public:	
	ADisappearingStone();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	bool bCanStandOnRock = true;

protected:
	virtual void BeginPlay() override;
	
//Player on Stone Vars
	FTimerHandle StandOnTimer;
	UFUNCTION()
	void PlayerOnStone();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StandOnTime = 2.0f;

//Player off Stone Vars
	FTimerHandle DisappearTimer;
	void StoneDisappeared();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReAppearTime = 2.0f;

/* Dynamic Material & Timeline */
	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	UCurveFloat* OpacityCurve;

	FTimeline OpacityTimeline;

	//Callbacks
	UFUNCTION()
	void HandleTimelineProgress(float Value);
	UFUNCTION()
	void HandleTimelineFinished();

	
/* Box Overlaps */	
	UFUNCTION(BlueprintCallable)
	void BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void BoxCompEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	
	
	
public:	
	virtual void Tick(float DeltaTime) override;

};
