// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Characters/CharacterTypes.h"
#include "Weapon.generated.h"


class USoundBase;
class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

	AWeapon();

public:
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	//this Array will store all of the actor that got hit with the attack and ignore them so they won't be hit again. 
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere)
	ECharacterState WeaponState;

	/*
	*Animation Montages for the weapons
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnShpererEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const {return WeaponBox; }
	
};
