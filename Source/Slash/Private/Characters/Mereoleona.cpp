// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mereoleona.h"
#include "GameFramework/SpringArmComponent.h" // to be able to add Spring Arm Component you need to include this herder
#include "Camera/CameraComponent.h" // to be able to add Camera Compoenent you need to include this Header. 
#include "GameFramework/CharacterMovementComponent.h" // to be able to to use Character Movement Compoenent variables u need to include this header.
#include "Components/StaticMeshComponent.h"
#include "GroomComponent.h" // you need to include this header to use the Groom components functions and variables. also you need to add HairStrandsCore to .build.cs
#include "EnhancedInputSubsystems.h" // To Be Able to Include this Header File you need to Adusjt .Build.cs file and add "EnhancedInput" in there. 
#include "EnhancedInputComponent.h" // to be able to Enhanced Input Action delation u need to use this header 
#include "Components/InputComponent.h" // to be able to Enhanced Input Action delation u need to use this header 
#include "Items/Item.h"	// we need to include both Item.h and Weapon.h for us to be able to use E Equio Function. 
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h" // you need this header file to access the AnimMontage Functionality. 


AMereoleona::AMereoleona()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw= false;
	bUseControllerRotationRoll= false; 

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 560.f, 0.f); 

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

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

void AMereoleona::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMereoleona::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMereoleona::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMereoleona::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMereoleona::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMereoleona::Attack);
	}
}

void AMereoleona::Jump()
{
	if ((ActionState == EActionState::EAS_Attacking) || (ActionState == EActionState::EAS_EquippingWeapon))
	{
		return;
	}
	if (bCanJump)
	{
		Super::Jump();
		bCanJump = false;
		bJumping = true;
	}
}

void AMereoleona::GetHit_Implementation(const FVector& ImpactPoint)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AMereoleona::BeginPlay()
{
	Super::BeginPlay();

	CreateEnhancedInput();

	Tags.Add(FName("EngageableTarget"));

}

void AMereoleona::CreateEnhancedInput()
{
	//Crateing Enchanced Input Local Player Subsystem and Connecting it to the Mapping Context Call "Slash".  
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Enabling Input Ensures that input system is Active. 
		EnableInput(PlayerController);


		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}
}

void AMereoleona::EquipWeapon(AWeapon* Weapon)
{
	// then we get the pointer and of tyoe AWeapon and access the Function Equip and pass in The mesh and the name of the socket.
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	//after Equipping the weapon we are setting the state of the Character to be EquippedOneHandedWeapon which we can use to set the animation blueprint 
	CharacterState = Weapon->WeaponState;
	EquippedWeapon = Weapon;
	OverlappingItem = nullptr;
	PossessState = EEquippingState::EES_Equipped;
	InitAttackMontage(EquippedWeapon);
}

void AMereoleona::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AMereoleona::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AMereoleona::CanDisarm()
{
	return (ActionState == EActionState::EAS_Unoccupied) && 
	(CharacterState != ECharacterState::ECS_Unequipped) &&
	(bJumping == false);
}

bool AMereoleona::CanArm()
{
	return (ActionState == EActionState::EAS_Unoccupied) && 
	(CharacterState == ECharacterState::ECS_Unequipped) && 
	(EquippedWeapon != nullptr) &&
	(bJumping == false);
}

void AMereoleona::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AMereoleona::Arm()
{
	PlayEquipMontage(FName("Equip"));
	if (EquippedWeapon)
	{
		CharacterState = EquippedWeapon->WeaponState;
	}
	else
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AMereoleona::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AMereoleona::AttackStart()
{
	ActionState = EActionState::EAS_Attacking;
}

void AMereoleona::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}

}

void AMereoleona::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AMereoleona::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMereoleona::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}
	
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

void AMereoleona::EKeyPressed()
{
	// To Equip a weapon onto the character mesh we first get the AWeapon pointer type and make a variable, then we Cast<AWeapon>(And assign the OverLapping function)
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	// then we check the pointer to make sure it is not a nullptr.
	if (OverlappingWeapon && (PossessState == EEquippingState::EES_Unequipped))
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
	
}

void AMereoleona::Attack()
{	
	Super::Attack();
	const bool bCanAttack = CanAttack() && bJumping == false;
							
	if (bCanAttack)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	} 
}
