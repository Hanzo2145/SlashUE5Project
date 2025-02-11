// Fill out your copyright notice in the Description page of Project Settings.
#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h" // you need this header file to abe able to create a Capsule Component.
#include "Components/SkeletalMeshComponent.h" // U need this header File to be able to create a Skeletal Mesh. 
#include "EnhancedInputSubsystems.h" // To Be Able to Include this Header File you need to Adusjt .Build.cs file and add "EnhancedInput" in there. 
#include "EnhancedInputComponent.h" // to be able to Enhanced Input Action delation u need to use this header 
#include "Components/InputComponent.h" // to be able to Enhanced Input Action delation u need to use this header 
#include "GameFramework/SpringArmComponent.h" // to be able to add Spring Arm Component you need to include this herder
#include "Camera/CameraComponent.h" // to be able to add Camera Compoenent you need to include this Header. 


ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	// to Create a Capsule Component you need to assigh the pointer variable to "CreateDefaultSubobject" and Spesify the Component <UCapsuleComponent>
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(Capsule); // To make the Capsule the Root Component
	Capsule->SetCapsuleHalfHeight(20.f); // This is how to set values for the Height of the Calpsule in C++
	Capsule->SetCapsuleRadius(20.f); // This is how you set the Radius of the capsule in C++

	//To Create a Skeletal Mesh in C++ U need to To create a Pointer And then Assign it to "CreateDefualtSubobject<USkeletalMeshComponent>(TEXT("Enther the name of the Compoenent Here"))"
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	BirdMesh->SetupAttachment(GetRootComponent()); // This is how you link Skeletal Mesh to the Root Compeonet So they can move together. 


	// To create a SpringArm you follow the formela below. 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArm->SetupAttachment(GetRootComponent()); // attach the spring arm to the Root Component.
	SpringArm->TargetArmLength = 400.f; // Set the Target Spring arm to be 400.
	SpringArm->bUsePawnControlRotation = true; // Make the Camera follow the Player controller. 

	// To create a Camear Component you follow the formela below.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm); // attach the camera to the spring arm.
	
	//Auto Pessessing Player. 
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Enable Controller Rotation. 
	bUseControllerRotationYaw = true; 


}

void ABird::BeginPlay()
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
 
void ABird::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	
	if ((!MoveValue.IsNearlyZero()) && (GetController())) // Optional: Check if there's actual movement input
	{
		// Getting the Forward Vector and Right Vector and Assigning them to a valuse.
		FVector Forward = GetActorForwardVector();
		FVector Right = GetActorRightVector();

		// Using the Function AddMovementInput() we are using forward vector and the right vector to move the pawn.  
		AddMovementInput(Forward, MoveValue.Y);
		AddMovementInput(Right, MoveValue.X);
	}
	
}

void ABird::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisValue = Value.Get<FVector2D>();

    if ((!LookAxisValue.IsNearlyZero()) && (GetController())) // Prevent unnecessary calculations
    {
        AddControllerYawInput(LookAxisValue.X);  // Using AddControllerYawInput we can now Turn Left/Right
        AddControllerPitchInput(LookAxisValue.Y); // Using AddControllerYawInput we can now Turn up/down
    }
}

void ABird::LookUpStart(const FInputActionValue& Value)
{
	//When Pressing the Right Mouse Button we can go Enable Use Controller Rotation Pitch Allowing us to go up and down.
    bUseControllerRotationPitch = true;
}

void ABird::LookUpEnd(const FInputActionValue& Value)
{
	//When lefting out finger form the right mouse button we disable Use Controller Rotation Pitch. 
    bUseControllerRotationPitch = false;
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);


		//this is allow us to controll the camera and out movement in the air. if we press the Right Mouse button we can fly UP/DOWN.
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Started, this, &ABird::LookUpStart);
        EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Completed, this, &ABird::LookUpEnd);
    }
}