#pragma once

// to create a enume in Unreal Engine we use the scope operator enume class E followed by the name of the Enum.
//to expose this Enum to blueprint we use this specefier called UENUM(BlueprintType)
UENUM(BlueprintType)
enum class ECharacterState : uint8
{	
	// we can make things more pretty for the blueprints and change their displayname. we do so by using the Specifier UMETA(DisplayName = "Displayneme"). 
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping a Weapon")
};