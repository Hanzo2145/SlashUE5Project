// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterTypes.h" //make sure to include the header file to make sure u can create the enum
#include "Mereoleona.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class SLASH_API AMereoleona : public ACharacter
{
	GENERATED_BODY()
	
public:
	AMereoleona();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;


	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bCanJump = true; 

	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool bcan) {bCanJump = bcan;}

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bJumping = false;

	UFUNCTION(BlueprintCallable)
	void SetJumping(bool bactive) {bJumping = bactive;}



protected:
	virtual void BeginPlay() override;

	/*
	* Add Input Actions 
	*/
	// Added Mapping Context to the Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* SlashMappingContext; 

	//Added an Input Action Move to the Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	void Move(const FInputActionValue& Value);

	// Added An Input Action Look to the blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	void Look(const FInputActionValue& Value); 
	
	// Added An Input Action Jump to the blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	// Added An Input Action Equip to the blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EquipAction;
	void EKeyPressed(); 

	// Added An Input Action Equip to the blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;
	void Attack(); 

	/*
		Play Montage Functions
	*/
	void PlayAttackMontage();

	/*
		Funtions and Variables that can decide things.
	*/
	//this will check to see if the attack ended and set the Action State to be Unoccupied
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	// this fucntion check and see if the player is not occupied with an action and if he has a weapon on.
	bool CanAttack();
	// this will set the ACtion state to be Attacking.
	UFUNCTION(BlueprintCallable)
	void AttackStart();

	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();


private:
	// Spring Arm Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm; 

	// Camera Compronent
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	//Adding Hair Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGroomComponent* Hair; 

	// Adding Eyebrows Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	 

	//to create a variable of type custom Enum we decalar the Enum type followed by the Name of the variable and then assign it to the enum value. 
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;


	/*
	* Animation Montages
	*/
	//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;



public:
	//note: we have to use FORCEINLINE when create a setter and getters to force the compile to accept these functions.
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } 

	//we are making a public getter for the variable Character State so we can use it inside of the SlashAnimInstance
	// A good policy is to make the getters const since u don't want them to change anything but only report what happened.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


};
