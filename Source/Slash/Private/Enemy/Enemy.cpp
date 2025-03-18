
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
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

#include "Slash/DebugMacros.h"
#include <Characters/Mereoleona.h>





AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
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
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		// Outside Combat Radius, lose interest
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = PatrolingSpeed; 
		MoveToTarget(PatrolTarget);
		//UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		// OutSide attack Range, Chase Character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		MoveToTarget(CombatTarget);
		//UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		// InSide Attack Range, attack Character
		EnemyState = EEnemyState::EES_Attacking;
		//UE_LOG(LogTemp, Warning, TEXT("Attack"));
		// TODO: AttackMontage
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	//Draw_Sphere_Color(ImpactPoint, FColor::Cyan); 
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	//lower impact point to the enemy actor location Z
	const FVector ImpactLowerd(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowerd - GetActorLocation()).GetSafeNormal();

	// Forward * toHit = |Forward| * |ToHit| * cos(Theta)
	// |Forward| = 1 |ToHit| = 1, so Forward * ToHit = cos(Theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// take the inverse cosine (arc-Cosine) of cos theta to get theta.
	double Theta = FMath::Acos(CosTheta);

	//convert from Radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);


	// if corss produt points down, Theta Should be Negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);

	
	//debug stuff down here to see from where they enemy is getting hit.
	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount); 
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		
	}
	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget); 
	return DamageAmount;
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
	
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(0, 5);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;

			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5 :
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

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
	MoveRequest.SetAcceptanceRadius(15.f);
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

void AEnemy::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Perception: Saw actor %s"), *Actor->GetName());
		if (EnemyState == EEnemyState::EES_Chasing) return;
		if (Actor->ActorHasTag(FName("SlashCharacter")))
		{
			GetWorldTimerManager().ClearTimer(PatrolTimer);
			GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
			CombatTarget = Actor;
			if (EnemyState != EEnemyState::EES_Attacking)
			{
				EnemyState = EEnemyState::EES_Chasing;
				MoveToTarget(CombatTarget);
			}
		}
	}
}