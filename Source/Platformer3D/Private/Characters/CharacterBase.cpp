
#include "Characters/CharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
//Inputs
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
/* Basic Components */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->SetRelativeLocation(FVector(-10.f, 0.f, 110.f));
		SpringArm->bUsePawnControlRotation = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
		CameraComp->SetupAttachment(SpringArm);
		CameraComp->bUsePawnControlRotation = false;
	
	//Character turn in move direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	//Character won't turn with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


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
	//PlayerController Ref + Input mode
	 PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerControllerRef)
		{
			PlayerControllerRef->SetInputMode(FInputModeGameOnly());
			PlayerControllerRef->SetShowMouseCursor(false);
		}

	//Add Mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer()))
	{
		if (PlayerMappingContext) Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}

	//Bind Input Actions to Functions
	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
		EIC->BindAction(LookInput, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
		EIC->BindAction(JumpInput, ETriggerEvent::Started, this, &ACharacterBase::JumpNow);
	}
	
}

void ACharacterBase::OnHit_Implementation()
{
	IHitInterface::OnHit_Implementation();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Hit");
	Die();
}

void ACharacterBase::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

/* Input Functions */
void ACharacterBase::Move(const FInputActionValue &Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	if (PlayerControllerRef && Input.SizeSquared() > 0.f)
	{
		const FRotator ControlRotation = PlayerControllerRef->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		//Find Forward and Right vectors relative to the player's camera rotation
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, Input.X); // X = Forward
		AddMovementInput(Right, Input.Y);   // Y = Right

		// Allows the player to rotate the character with input (WASD),
		// and move forward in the direction the camera is looking.
	}
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void ACharacterBase::JumpNow(const FInputActionValue& Value)
{
	ACharacterBase::Jump();
}
