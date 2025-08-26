// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"


#include "CharacterBase.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PLATFORMER3D_API ACharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Hit Interface
	virtual void OnHit_Implementation() override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;
	UFUNCTION(BlueprintCallable)
	void Die();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	
	UFUNCTION(BlueprintCallable)
	void Respawn();

	//Timer
	FTimerHandle RespawnTimer;
	void RespawnPlayer();
	
	UPROPERTY()
	UCharacterMovementComponent* MoveComp;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerController* PlayerControllerRef = nullptr;
	
/* Basic Components */
	UPROPERTY(EditAnywhere, Category = Components)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Components)
	UCameraComponent* CameraComp;
/* Clothing Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Hat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Face;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Glasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Mustache;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Pants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Shoes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* Tops;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* ClownNose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Cloths")
	USkeletalMeshComponent* FullBody;

	
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


	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* PauseInput;

	//Input Functions
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void JumpNow(const FInputActionValue &Value);

	void PauseGame(const FInputActionValue &Value);
/* ~End of Inputs */
};
