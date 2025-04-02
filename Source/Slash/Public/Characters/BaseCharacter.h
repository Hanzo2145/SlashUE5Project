#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces\HitInterface.h"
#include "Characters/CharacterTypes.h"
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
	/** <AActor>*/
	virtual void Tick(float DeltaTime) override;
	/** </AActor>*/


protected:
	/** <AActor>*/
	virtual void BeginPlay() override;
	/** </AActor>*/

	/*<HitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/*</HitInterface>*/

	/** <ABaseCharacter>*/
	virtual void Attack();
	virtual void Die();
	virtual void HandleDamage(float DamageAmount);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual bool CanAttack();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	/** </ABaseCharacter>*/

	void InitAttackMontage(AWeapon* NewWeapon);
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	bool IsAlive();
	void DisableCapsule();
	void StopAttackMontage();
	void DisableMeshCollision();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

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

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

private:

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	UPROPERTY()
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
public:

	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose()const { return DeathPose; }
};

