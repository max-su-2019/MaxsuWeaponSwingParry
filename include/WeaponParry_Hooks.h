#pragma once

namespace MaxsuWeaponParry
{
	class MeleeHitHook
	{

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);

			REL::Relocation<std::uintptr_t> OnMeleeHitBase{ REL::ID(37650) };
			auto& trampoline = SKSE::GetTrampoline();
			_OnMeleeHit = trampoline.write_call<5>(OnMeleeHitBase.address() + 0x38B, OnMeleeHit);

			logger::info(FMT_STRING("OnMeleeHit Hook Install! Base {}, Offset {:x}"), 37650, 0x38B);
		}


	private:
		static void OnMeleeHit(RE::Actor* attacker, RE::Actor* target, std::int64_t a_int1, bool a_bool, void* a_unkptr);

		static inline REL::Relocation<decltype(OnMeleeHit)> _OnMeleeHit;

		static bool SendRecoil(RE::Actor* thisactor)
		{
			bool result = false;

			if (thisactor && thisactor->GetGraphVariableBool("IsRecoiling", result) && result == false) {
				thisactor->NotifyAnimationGraph("recoilStart");
				return true;
			}

			return false;
		}
	};

}