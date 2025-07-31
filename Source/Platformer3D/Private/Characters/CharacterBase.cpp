
#include "Characters/CharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
/* Basic Components */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->SetRelativeLocation(FVector(-10.f, 0.f, 110.f));
	CameraComp = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
		CameraComp->SetupAttachment(SpringArm);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

