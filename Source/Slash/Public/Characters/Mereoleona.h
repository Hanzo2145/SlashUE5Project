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



protected:
	virtual void BeginPlay() override;

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
	void Equip(); 

	// Added An Input Action Equip to the blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;
	void Attack(); 

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
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;


	/*
	* Animation Montages
	*/
	//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;



public:
	//note: we have to use FORCEINLINE when create a setter and getters to force the compile to accept these functions.
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } 

	//we are making a public getter for the variable Character State so we can use it inside of the SlashAnimInstance
	// A good policy is to make the getters const since u don't want them to change anything but only report what happened.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


};
