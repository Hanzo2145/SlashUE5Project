// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DecelerationSpeed = DeltaTime * DecelerationRate;

	if (GetActorLocation().Z > DesiredZ)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, DecelerationSpeed));
	}
}


void ASoul::BeginPlay()
{
	Super::BeginPlay();
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
		);
	FVector ImpactPoint = HitResult.ImpactPoint;
	DesiredZ = ImpactPoint.Z + SoulZLocation;
}


void ASoul::OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);
		SpawnPickupSystem();
		SpawnPickupSound();
		Destroy();
	}
	
	
}
