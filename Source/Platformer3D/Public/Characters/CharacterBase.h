// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PLATFORMER3D_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

/* Basic Components */
	UPROPERTY(EditAnywhere, Category = Components)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Components)
	UCameraComponent* CameraComp;


};
