// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "RotatingObstacle.generated.h"

class URotatingMovementComponent;
class UCapsuleComponent;

UCLASS()
class PLATFORMER3D_API ARotatingObstacle : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ARotatingObstacle();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	URotatingMovementComponent* RotatingMovementComp;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComp;


protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BLueprintCallable)
	void CallOnHitInterface(AActor* HitActor);
};
