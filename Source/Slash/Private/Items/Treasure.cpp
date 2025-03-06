// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters\Mereoleona.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// To make the character equip a weapon first we cast to the player by making a pointer of type Character then assinging it to the OtherActor. making sure the other actor is Character.
	AMereoleona* SlashCharacter = Cast<AMereoleona>(OtherActor);

	// then we check the pointer value to make sure it is not a nullptr
	if (SlashCharacter)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		Destroy();
	}
}
