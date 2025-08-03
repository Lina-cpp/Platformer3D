// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

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

	UPROPERTY()
	APlayerController* PlayerControllerRef = nullptr;
	
/* Basic Components */
	UPROPERTY(EditAnywhere, Category = Components)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Components)
	UCameraComponent* CameraComp;
/* ~End of Basic Components */
/* Inputs */
	//IMC
	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputMappingContext* PlayerMappingContext;

	//ActionInputs
	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* MoveInput;
	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* LookInput;
	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* JumpInput;




	//Input Functions
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void JumpNow(const FInputActionValue &Value);
/* ~End of Inputs */
};
