// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"

void UCombatGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	const FGameplayTag& DualWieldTag = Crys::NativeGameplayTag::Ability_State_DualWielding;
	FOnGameplayEffectTagCountChanged::FDelegate Delegate = FOnGameplayEffectTagCountChanged::FDelegate::CreateWeakLambda(this,
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			bDualWielding = NewCount > 0;
		});
	GetAbilitySystemComponentFromActorInfo()->RegisterAndCallGameplayTagEvent(DualWieldTag, Delegate);
	
	CombatComponent = GetOwningActorFromActorInfo()->FindComponentByClass<UCombatSystemComponent>();
}
