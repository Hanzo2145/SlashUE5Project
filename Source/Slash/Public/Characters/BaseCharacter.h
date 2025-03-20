#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces\HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();

	void InitAttackMontage(AWeapon* NewWeapon);
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount); 
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void DisableCapsule();


	/*
	* Attack Functions
	*/
	// this fucntion check and see if the player is not occupied with an action and if he has a weapon on.
	virtual bool CanAttack();
	bool IsAlive();
	//this will check to see if the attack ended and set the Action State to be Unoccupied
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	/*
	* Animation Montages
	*/
	UPROPERTY()
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	/*
	* Components
	*/
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;


	/*
	* Variables
	*/
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
	float DeathTimer = 3.f;
private:

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

};
