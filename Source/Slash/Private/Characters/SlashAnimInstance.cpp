// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/Mereoleona.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Mereoleona =  Cast<AMereoleona>(TryGetPawnOwner());
    if (Mereoleona)
    {
        SlashCharacterMovement = Mereoleona->GetCharacterMovement();
    }
    

}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (SlashCharacterMovement)
    {
        GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
        IsFalling = SlashCharacterMovement->IsFalling();
    }
    

}