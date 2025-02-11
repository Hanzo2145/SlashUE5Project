// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Prameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Prameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();


	UFUNCTION(BlueprintPure)
	float TransformedCos();

	UFUNCTION(BlueprintPure)
	float SpinRotation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spinning Rotation")
	float Speed = 5.f;

	float DeltaSecond; 

	UFUNCTION()
	void OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnShpererEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta =(AllowPrivateAccess= "true"))
	float RunningTime;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ItemMesh;
 

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Sphere;


};
