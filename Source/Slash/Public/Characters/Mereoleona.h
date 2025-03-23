// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
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
class SLASH_API AMereoleona : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMereoleona();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

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
	void CreateEnhancedInput();
	void EquipWeapon(AWeapon* Weapon);
	//this will check to see if the attack ended and set the Action State to be Unoccupied
	virtual void AttackEnd() override;
	// this fucntion check and see if the player is not occupied with an action and if he has a weapon on.
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);
	
	// this will set the ACtion state to be Attacking.	
	UFUNCTION(BlueprintCallable)
	void AttackStart();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

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
	virtual void Attack() override;	

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

	//to create a variable of type custom Enum we decalar the Enum type followed by the Name of the variable and then assign it to the enum value. 
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEquippingState PossessState = EEquippingState::EES_Unequipped; 


	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY()
	bool IsJumping;

public:
	//note: we have to use FORCEINLINE when create a setter and getters to force the compile to accept these functions.
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } 

	//we are making a public getter for the variable Character State so we can use it inside of the SlashAnimInstance
	// A good policy is to make the getters const since u don't want them to change anything but only report what happened.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


};
