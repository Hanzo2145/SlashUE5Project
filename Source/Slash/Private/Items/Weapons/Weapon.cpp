// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/Mereoleona.h" // you need to include the ACharacter.h so u can cast to it. and get the other Actor
#include "Characters/CharacterTypes.h"
#include "Kismet/GameplayStatics.h" // you need to Include this header file to be able to use sounds functions.
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

AWeapon::AWeapon()
{
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    WeaponBox->SetupAttachment(GetRootComponent()); 
}

void AWeapon::OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnShpererEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnShpererEndOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
    AttachMeshToSocket(InParent, InSocketName);
    ItemState = EItemState::EIS_Equipped;
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
    }
    
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
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