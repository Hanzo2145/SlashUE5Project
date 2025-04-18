// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
protected:
	
	virtual void BeginPlay() override;

private:

	// Current Health. 
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxHealth; 

	// Current Stamina. 
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float StaminaRegenRate = 5.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float HealingAmount;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	int32 HealingCharges = 5;

public:
	void ReceiveDamage(float Damage); 
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStamainaPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	float HealPlayer(float Amount);
	int32 SubHealingCharges(int32 Amount);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina;  }
	FORCEINLINE float GetHealingAmount() const { return HealingAmount; }
	FORCEINLINE float GetHealingCharges() const { return HealingCharges; }
		
};
