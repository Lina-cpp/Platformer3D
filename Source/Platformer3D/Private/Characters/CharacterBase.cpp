
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

/* Clothing components */
	Hat = CreateDefaultSubobject<USkeletalMeshComponent>("Hat");
		Hat->SetupAttachment(GetMesh());
	
	Face = CreateDefaultSubobject<USkeletalMeshComponent>("Face");
		Face->SetupAttachment(GetMesh());
	
	Glasses = CreateDefaultSubobject<USkeletalMeshComponent>("Glasses");
		Glasses->SetupAttachment(GetMesh());

	Mustache = CreateDefaultSubobject<USkeletalMeshComponent>("Mustache");
		Mustache->SetupAttachment(GetMesh());

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>("Pants");
		Pants->SetupAttachment(GetMesh());

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>("Shoes");
		Shoes->SetupAttachment(GetMesh());

	Tops = CreateDefaultSubobject<USkeletalMeshComponent>("Tops");
		Tops->SetupAttachment(GetMesh());

	ClownNose = CreateDefaultSubobject<USkeletalMeshComponent>("ClownNose");
		ClownNose->SetupAttachment(GetMesh());

	FullBody = CreateDefaultSubobject<USkeletalMeshComponent>("FullBody");
		FullBody->SetupAttachment(GetMesh());
	
/* Movement and Rotations */
	//Character turn in move direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	//Character won't turn with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	
}

void ACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
/* Clothing components attachment */
	Hat->SetLeaderPoseComponent(GetMesh());
	Face ->SetLeaderPoseComponent(GetMesh());
	Glasses->SetLeaderPoseComponent(GetMesh());
	Pants->SetLeaderPoseComponent(GetMesh());
	Mustache->SetLeaderPoseComponent(GetMesh());
	Shoes->SetLeaderPoseComponent(GetMesh());
	Tops->SetLeaderPoseComponent(GetMesh());
	ClownNose->SetLeaderPoseComponent(GetMesh());
	FullBody->SetLeaderPoseComponent(GetMesh());
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//Save MovementComponent
	MoveComp = GetCharacterMovement();

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
		EIC->BindAction(PauseInput, ETriggerEvent::Started, this, &ACharacterBase::PauseGame);
	}
	
}

void ACharacterBase::OnHit_Implementation()
{
	IHitInterface::OnHit_Implementation();

	//if character got hit - call Die()
	//otherwise leave function
	if (bIsDead) return;
	Die();
}

void ACharacterBase::Die()
{

	bIsDead = true;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	//Disable Movement but not inputs
	if (MoveComp) MoveComp->DisableMovement();
	else GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("MoveComp Invalid")));

	//Timer to respawn
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ACharacterBase::RespawnPlayer, 3.f, false);
}


void ACharacterBase::RespawnPlayer()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "RespawnPlayer");
	OnDeath();
}




void ACharacterBase::Respawn()
{
	bIsDead = false;

	GetMesh()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


	if (MoveComp) MoveComp->SetMovementMode(MOVE_Walking);
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

void ACharacterBase::PauseGame(const FInputActionValue& Value)
{
	
}
