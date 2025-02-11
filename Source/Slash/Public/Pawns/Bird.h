// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" // you need to Include this Header for the compilor to know about "FInputActionValue"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookUpAction; 
	void LookUpStart(const FInputActionValue& Value);
	void LookUpEnd(const FInputActionValue& Value);
	

private:
	
	// Capusle Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCapsuleComponent* Capsule; 

	// Skeletal Mesh Component
    UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* BirdMesh;

	// Spring Arm Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm; 

	// Camera Compronent
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

};
