// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/Mereoleona.h" // you need to include the ACharacter.h so u can cast to it. and get the other Actor
#include "Characters/CharacterTypes.h"
#include "Kismet/GameplayStatics.h" // you need to Include this header file to be able to use sounds functions.
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"
#include "Enemy/Enemy.h"

AWeapon::AWeapon()
{
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    WeaponBox->SetupAttachment(GetRootComponent()); 
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(GetRootComponent());
    
    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
    ItemState = EItemState::EIS_Equipped;
    SetOwner(NewOwner);
    WeaponOwner = NewOwner;
    SetInstigator(NewInstigator);
    AttachMeshToSocket(InParent, InSocketName);
    DisableSphereCollision();
    PlayEquipSound();
    DeactiveateEmbers();
}

void AWeapon::DeactiveateEmbers()
{
    if (EmbersEffect)
    {
        EmbersEffect->Deactivate();
    }
}

void AWeapon::DisableSphereCollision()
{
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AWeapon::PlayEquipSound()
{
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
    }
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    // now to attach the weapon to the socket we need to fist get an AttachmentTransformRules by using TransfromRules(passing in EAttachmentRule::SnapToTarget, true) 
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

    /*then we get the mesh and call AttachToComponent we and passing in (first we get the variable by using the pointer->GetMesh(), then TransformRule(), 
    then the name of the socket make sure it is Typed Right as it is Case sensative.)*/ 
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName); 
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ActorIsSameType(OtherActor)) return;
    FHitResult BoxHit;
    BoxTrace(BoxHit);
    AActor* HitActor = BoxHit.GetActor();
    if (IsValid(HitActor))
    {
        if (ActorIsSameType(HitActor)) return;
        
        UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
        ExecuteGetHit(BoxHit);
        CreateFields(BoxHit.ImpactPoint);
    }
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
    return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
    IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
    if (HitInterface)
    {
        HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
    }
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
    if (!BoxTraceStart || !BoxTraceEnd)
    {
        UE_LOG(LogTemp, Error, TEXT("BoxTraceStart or BoxTraceEnd is null!"));
        return;
    }

    AActor* MyOwner = GetOwner();
    if (!IsValid(MyOwner))
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon has no valid owner!"));
        return;
    }

    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetOwner());

    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    UKismetSystemLibrary::BoxTraceSingleForObjects(
        this,
        Start,
        End,
        BoxTraceExtent,
        BoxTraceStart->GetComponentRotation(),
        ObjectTypes, 
        false,       
        ActorsToIgnore,
        bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        BoxHit,
        true         
    );

    AActor* HitActor = BoxHit.GetActor();
    if (!IsValid(HitActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("BoxTrace found no valid actor!"));
        return;
    }
    IgnoreActors.AddUnique(BoxHit.GetActor());
}
