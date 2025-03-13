// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ObstacleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASkateCharacter

ASkateCharacter::ASkateCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create skate
	SkateComponent = CreateDefaultSubobject<USkateComponent>(TEXT("SkateComponent"));
	SkateComponent->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASkateCharacter::Tick(float DeltaSeconds)
{
	if (SkateComponent->IsMoving())
	{
		AddMovementInput(GetActorForwardVector(), SkateComponent->GetMovementSpeed());
	}
}

void ASkateCharacter::Jump()
{
	Super::Jump();

	TArray<FHitResult> Hits = GetHitsInLine(GetActorLocation(), GetActorLocation() + MaxObstacleDistance * GetActorForwardVector());
	for (FHitResult Hit : Hits)
	{
		UObstacleComponent* Obstacle = Hit.GetActor()->GetComponentByClass<UObstacleComponent>();
		if (IsValid(Obstacle))
		{
			AvailableObstacles.Add(Obstacle);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("JUMP: %d obstacles"), AvailableObstacles.Num())
}

void ASkateCharacter::StopJumping()
{
	Super::StopJumping();

	
}

void ASkateCharacter::Landed(const FHitResult& Hit)
{
	AvailableObstacles.Empty();
}

void ASkateCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddUniqueDynamic(this, &ASkateCharacter::OnHit);
	AvailableObstacles.Empty();
}

void ASkateCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (SkateComponent->IsMoving())
	{
		FVector EscapeDirection;
		SkateComponent->ProcessHit(Hit.Normal, EscapeDirection);

		if (!EscapeDirection.IsNearlyZero())
		{
			AddMovementInput(EscapeDirection, SkateComponent->GetMovementSpeed());
		}
	}
}

TArray<FHitResult> ASkateCharacter::GetHitsInLine(FVector LineStart, FVector LineEnd)
{
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> Hits;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), LineStart, LineEnd, ObstacleDetectionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hits, true);
	return Hits;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASkateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASkateCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASkateCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASkateCharacter::StopMoving);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkateCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASkateCharacter::Move(const FInputActionValue& Value)
{
	if (GetMovementComponent()->IsMovingOnGround())
	{
		FVector2D MovementDirection = Value.Get<FVector2D>();
		SkateComponent->SetMovementInput(MovementDirection);
		AddMovementInput(GetActorRightVector(), SkateComponent->GetTurnSpeed());
	}
}

void ASkateCharacter::StopMoving(const FInputActionValue& Value)
{
	SkateComponent->SetMovementInput(FVector2D::ZeroVector);
}

void ASkateCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}