#pragma once

namespace MaxsuWeaponParry
{
	class ParryCheck
	{
	public:

		ParryCheck() = delete;

		static bool ShouldParry(RE::Actor* hit_causer, RE::Actor* hit_target);

	private:
		static bool IsParryableWeapon(RE::TESForm* thisWeapon);

		static bool AttackingWithParryableWeapon(RE::AIProcess* a_process);
	};

}