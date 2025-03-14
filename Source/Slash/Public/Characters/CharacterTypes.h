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

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6")
};

UENUM(BlueprintType)
enum class EEquippingState : uint8
{
	// we can make things more pretty for the blueprints and change their displayname. we do so by using the Specifier UMETA(DisplayName = "Displayneme"). 
	EES_Unequipped UMETA(DisplayName = "Unequipped"),
	EES_Equipped UMETA(DisplayName = "Equipped")
};