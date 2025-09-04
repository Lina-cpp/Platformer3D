// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <rapidjson/internal/meta.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisappearingStone.generated.h"

class UBoxComponent;
class ACharacterBase;

UCLASS()
class PLATFORMER3D_API ADisappearingStone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADisappearingStone();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	bool bCanStandOnRock = true;

protected:
	virtual void BeginPlay() override;
	

	FTimerHandle StandOnTimer;
	UFUNCTION()
	void PlayerOnStone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StandOnTime = 2.0f;
	
	FTimerHandle DisappearTimer;
	void StoneDisappeared();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReAppearTime = 4.0f;

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

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap();
	UFUNCTION(BlueprintNativeEvent)
	void EndOverlap();
	
	
	
public:	
	virtual void Tick(float DeltaTime) override;

};
