
#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"


#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"

#include "Slash/DebugMacros.h"
#include <Characters/Mereoleona.h>
#include "Items/Weapons/Weapon.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Widget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (SightConfig)
	{
		SightConfig->SightRadius = 4000.f;
		SightConfig->LoseSightRadius = 4200.f;
		SightConfig->PeripheralVisionAngleDegrees = 45.f;

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
		AIPerceptionComponent->ConfigureSense(*SightConfig);
	}


	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrolingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	UE_LOG(LogTemp, Warning, TEXT("Start Attack Timer Was set, Enemy will attack again in %f"), AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (CombatTarget == DestroyedActor)
	{
		CombatTarget = nullptr;
		if (EnemyController)
		{
			EnemyController->StopMovement();
		}
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false); 
	}

	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnTargetPerceptionUpdated);

	}

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("SwordSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
		InitAttackMontage(DefaultWeapon);
	}

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
	
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax); 
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
		UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
		UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (CanAttack())
	{
		StartAttackTimer();
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();

	if (IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else 
	{
		Die();
	}
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AEnemy::HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (CombatTarget)
	{
		CombatTarget->OnDestroyed.AddDynamic(this, &AEnemy::OnTargetDestroyed);
	}
	ChaseTarget();
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::Die()
{
	if (EnemyController)
	{
		EnemyController->StopMovement();
	}
	EnemyState = EEnemyState::EES_Dead;
	PlayDeathMontage();
	ClearAttackTimer();
	DisableCapsule();
	HideHealthBar();
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 RandomTarget = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[RandomTarget];
	}
	return nullptr;
}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	Super::Attack();
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection; 
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();

}

void AEnemy::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		!IsChasing() &&
		EnemyState < EEnemyState::EES_Attacking &&
		Actor->ActorHasTag(FName("SlashCharacter"));

	if (Stimulus.WasSuccessfullySensed())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Perception: Saw actor %s"), *Actor->GetName());
		if (bShouldChaseTarget)
		{
			CombatTarget = Actor;
			ClearPatrolTimer();
			ChaseTarget();
		}
	}
}