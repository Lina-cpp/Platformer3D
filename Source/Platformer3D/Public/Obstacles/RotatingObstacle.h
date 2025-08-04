// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingObstacle.generated.h"

class URotatingMovementComponent;

UCLASS()
class PLATFORMER3D_API ARotatingObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	ARotatingObstacle();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere)
	URotatingMovementComponent* RotatingMovementComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

protected:
	virtual void BeginPlay() override;

};
