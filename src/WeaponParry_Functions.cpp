#include "WeaponParry_Functions.h"

namespace MaxsuWeaponParry
{

	bool ParryCheck::IsParryableWeapon(RE::TESForm* thisWeapon)
	{
		using type = RE::WEAPON_TYPE;

		if (!thisWeapon){
			logger::debug("Weapon Form Not Found!");
			return false;
		}

		auto Weap = thisWeapon->As<RE::TESObjectWEAP>();

		if (!Weap){
			logger::debug("Weapon Obj Not Found!");
			return false;
		}

		return (!Weap->IsHandToHandMelee() && !Weap->IsOneHandedDagger() && Weap->IsMelee());
	}


	bool ParryCheck::AttackingWithParryableWeapon(RE::AIProcess* a_process)
	{
		using FLAG = RE::AttackData::AttackFlag;

		if (a_process && a_process->high && a_process->high->attackData && !a_process->high->attackData->data.flags.all(FLAG::kBashAttack)) 	
			return (!a_process->high->attackData->IsLeftAttack() && IsParryableWeapon(a_process->GetEquippedRightHand())) ||
				   (a_process->high->attackData->IsLeftAttack() && IsParryableWeapon(a_process->GetEquippedLeftHand()));

		return false;
	}


	bool ParryCheck::ShouldParry(RE::Actor* hit_causer, RE::Actor* hit_target)
	{
		using ATTACK_STATE = RE::ATTACK_STATE_ENUM;
		//----------------------Check Hit Target-------------------------------------------------------

		if (!hit_target) {
			logger::debug("Hit Target Not Found!");
			return false;
		}

		if (!hit_target->Is3DLoaded()) {
			logger::debug("Hit Target Not 3D Loaded!");
			return false;
		}

		logger::debug(FMT_STRING("Hit Target Name is \"{}\", ID is \"{:x}\""), hit_target->GetName(), hit_target->GetFormID());

		if (hit_target->formType != RE::FormType::ActorCharacter) {
			logger::debug("Hit Target Not Actor!");
			return false;
		}

		if (hit_target->IsDead()) {
			logger::debug("Hit Target is Dead!");
			return false;
		}

		if (hit_target->IsGhost() || hit_target->HasEffectWithArchetype(RE::EffectArchetypes::ArchetypeID::kEtherealize)) {
			logger::debug("Hit Target Is Ghost or Etherealize!");
			return false;
		}

		if (hit_target->GetAttackState() != ATTACK_STATE::kSwing || hit_target->IsBlocking()) {
			logger::debug("Hit Target Is Not Weapon Swing Parrying!");
			return false;
		}

		if (!hit_target->currentProcess) {
			logger::debug("Hit Target AI Process Not Found!");
			return false;
		}

		if (!ParryCheck::AttackingWithParryableWeapon(hit_target->currentProcess)) {
			logger::debug("Hit Target Not using Parryable Weapon!");
			return false;
		}

		//-----------------------------------------------------------------------------------------------


		//----------------------Check Hit Causer-----------------------------------------------------------

		if (!hit_causer) {
			logger::debug("Hit Causer Not Found!");
			return false;
		}

		if (!hit_causer->Is3DLoaded()) {
			logger::debug("Hit Causer Not 3D Loaded!");
			return false;
		}

		logger::debug(FMT_STRING("Hit Causer Name is \"{}\", ID is \"{:x}\""), hit_causer->GetName(), hit_causer->GetFormID());

		if (hit_causer->formType != RE::FormType::ActorCharacter) {
			logger::debug("Hit Causer Not Actor!");
			return false;
		}

		if (hit_causer->IsDead()) {
			logger::debug("Hit Causer is Dead!");
			return false;
		}

		if (hit_causer->IsGhost() || hit_causer->HasEffectWithArchetype(RE::EffectArchetypes::ArchetypeID::kEtherealize)) {
			logger::debug("Hit Causer Is Ghost or Etherealize!");
			return false;
		}

		if (!hit_causer->currentProcess) {
			logger::debug("Hit Causer AI Process Not Found!");
			return false;
		}

		if (!ParryCheck::AttackingWithParryableWeapon(hit_causer->currentProcess)) {
			logger::debug("Hit Causer Not using Parryable Weapon!");
			return false;
		}
		//-------------------------------------------------------------------------------------------------------

		return true;
	}

}