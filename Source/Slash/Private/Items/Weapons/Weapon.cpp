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

void AWeapon::OnBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (ActorIsSameType(OtherActor)) return;
    
    FHitResult BoxHit;
    BoxTrace(BoxHit);

    if (BoxHit.GetActor())
    {
        if (ActorIsSameType(BoxHit.GetActor())) return;
        UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
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
        IHitInterface::Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
    }
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    /* this for loop will add all the actor that got hit to the variable ActorsToIgnore which we pass to the BoxTraceSingle
    this way we won't hit the same enemy twice per hit.  */
    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    ActorsToIgnore.Add(this);
    UKismetSystemLibrary::BoxTraceSingle(this,
        Start,
        End,
        BoxTraceExtent,
        BoxTraceStart->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false, ActorsToIgnore,
        bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, 
        BoxHit, 
        true
    );
    IgnoreActors.AddUnique(BoxHit.GetActor());

}
