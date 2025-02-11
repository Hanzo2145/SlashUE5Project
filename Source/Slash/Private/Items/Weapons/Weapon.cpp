// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/Mereoleona.h" // you need to include the ACharacter.h so u can cast to it. and get the other Actor


void AWeapon::OnSphereOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


    // to Attach a weapon to a character we first include the Character header file then we cast to that charcter using Cast<>(OtherActor); 
    // after that we store it in a pointer of value pointer to the character class. 
    AMereoleona* SlashCharacter = Cast<AMereoleona>(OtherActor);

    // we check to see if the cast was seccessful then we do the atttachment.
    if (SlashCharacter)
    {

        // now to attach the weapon to the socket we need to fist get an AttachmentTransformRules by using TransfromRules(passing in EAttachmentRule::SnapToTarget, true) 
        FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

        /*then we get the mesh and call AttachToComponent we and passing in (first we get the variable by using the pointer->GetMesh(), then TransformRule(), 
        then the name of the socket make sure it is Typed Right as it is Case sensative.)*/ 
        ItemMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));
    }
    

}


void AWeapon::OnShpererEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnShpererEndOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}