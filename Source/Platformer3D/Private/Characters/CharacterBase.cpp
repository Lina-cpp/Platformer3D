
#include "Characters/CharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
//Inputs
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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

	//Character won't turn with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Character turn in move direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 100.f, 0.f);
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
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
		}

	//Add Mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if (PlayerMappingContext) Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}

	//Bind Input Actions to Functions
	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
		EIC->BindAction(LookInput, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
	}
	
}
/* Input Functions */
void ACharacterBase::Move(const FInputActionValue &Value)
{
	
	const FVector2D Direction = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector() * Direction.X);
	AddMovementInput(GetActorRightVector() * Direction.Y);
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("Look")));
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}
