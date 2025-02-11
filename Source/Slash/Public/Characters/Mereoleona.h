// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Mereoleona.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

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


};
