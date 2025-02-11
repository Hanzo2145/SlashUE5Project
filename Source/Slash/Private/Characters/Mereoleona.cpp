// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mereoleona.h"
#include "Components/CapsuleComponent.h" // you need this header file to abe able to create a Capsule Component.
#include "Components/SkeletalMeshComponent.h" // U need this header File to be able to create a Skeletal Mesh. 
#include "EnhancedInputSubsystems.h" // To Be Able to Include this Header File you need to Adusjt .Build.cs file and add "EnhancedInput" in there. 
#include "EnhancedInputComponent.h" // to be able to Enhanced Input Action delation u need to use this header 
#include "Components/InputComponent.h" // to be able to Enhanced Input Action delation u need to use this header 
#include "GameFramework/SpringArmComponent.h" // to be able to add Spring Arm Component you need to include this herder
#include "Camera/CameraComponent.h" // to be able to add Camera Compoenent you need to include this Header. 
#include "GameFramework/CharacterMovementComponent.h" // to be able to to use Character Movement Compoenent variables u need to include this header.
#include "GroomComponent.h" // you need to include this header to use the Groom components functions and variables. also you need to add HairStrandsCore to .build.cs


AMereoleona::AMereoleona()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw= false;
	bUseControllerRotationRoll= false; 

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 560.f, 0.f); 

	// Created a SpringArm 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArm->SetupAttachment(GetRootComponent()); // attach the spring arm to the Root Component.
	SpringArm->TargetArmLength = 400.f; // Set the Target Spring arm to be 400.
	SpringArm->bUsePawnControlRotation = true; // Make the Camera follow the Player controller.

	// Created a Camear Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm); // attach the camera to the spring arm.

	//Added the Hair Component 
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair Component"));
	Hair->SetupAttachment(GetMesh()); // to be able to attach the hair to the mesh you need to get the public getter "GetMesh()"
	Hair->AttachmentName = FString("Head"); // to attach the hair to a sokect you use AttachmentName and assinging it to FString and the name of the socket.

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows Component"));
	Eyebrows->SetupAttachment(GetMesh()); // to be able to attach the Eyebrows to the mesh you need to get the public getter "GetMesh()"
	Eyebrows->AttachmentName = FString("Head"); // to attach the Eyebrows to a sokect you use AttachmentName and assinging it to FString and the name of the socket.
	

}

void AMereoleona::BeginPlay()
{
	Super::BeginPlay();

	//Crateing Enchanced Input Local Player Subsystem and Connecting it to the Mapping Context Call "Slash".  
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Enabling Input Ensures that input system is Active. 
		EnableInput(PlayerController);


		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashMappingContext, 0);
		}	
	}
}

void AMereoleona::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} 

void AMereoleona::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	 if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMereoleona::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMereoleona::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMereoleona::Jump);
    }
}

void AMereoleona::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirecation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirecation, MoveValue.Y);

	const FVector RightDirecation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirecation, MoveValue.X);
}

void AMereoleona::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisValue = Value.Get<FVector2D>();

    if ((!LookAxisValue.IsNearlyZero()) && (GetController())) // Prevent unnecessary calculations
    {
        AddControllerYawInput(LookAxisValue.X);  // Using AddControllerYawInput we can now Turn Left/Right
        AddControllerPitchInput(LookAxisValue.Y); // Using AddControllerYawInput we can now Turn up/down
    }
}


void AMereoleona::Jump()
{
	if (bCanJump)
	{
		Super::Jump();
		bCanJump = false;
	}
}

