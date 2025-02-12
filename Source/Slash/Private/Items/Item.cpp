// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/Mereoleona.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh Component"));
	RootComponent = ItemMesh; 

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Shpere"));
	Sphere->SetupAttachment(GetRootComponent());


}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverLap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnShpererEndOverLap);
}

float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

float AItem::SpinRotation()
{
    return DeltaSecond * Speed;
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	DeltaSecond = DeltaTime;

}


void AItem::OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// To make the character equip a weapon first we cast to the player by making a pointer of type Character then assinging it to the OtherActor. making sure the other actor is Character.
	AMereoleona* SlashCharacter = Cast<AMereoleona>(OtherActor);

	// then we check the pointer value to make sure it is not a nullptr
	if (SlashCharacter)
	{
		// if it successed we get the pointer and and use the Seter function we made and pass this as the AItem pointer which we made in the Character class. 
		SlashCharacter->SetOverlappingItem(this); 
	}
	
}


void AItem::OnShpererEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// in EndOverlap we are just removing the pointer and setting to nullptr
	AMereoleona* SlashCharacter = Cast<AMereoleona>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr); 
	}
	
}
