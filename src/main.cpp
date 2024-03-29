﻿#include "version.h"
#include "WeaponParry_Hooks.h"
#include "MaxsuLogsBackUp.h"

#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\MaxsuWeaponParry.ini"

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "MaxsuWeaponParry.log"sv;
	MaxsuLogsBackUpNamespace::CreateBackUpLogs(*path, 3);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::trace);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("MaxsuWeaponParry v{}"), MYFP_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "MaxsuWeaponParry";
	a_info->version = MYFP_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("MaxsuWeaponParry loaded");

	SKSE::Init(a_skse);

	auto g_message = SKSE::GetMessagingInterface();

	if (!g_message)
	{
		logger::error("Messaging Interface Not Found!");
		return false;
	}

	MaxsuWeaponParry::MeleeHitHook::InstallHook();

	auto IsEnableDebugMessage = GetPrivateProfileIntA("Main", "EnableDebugMessage", 0, SETTINGFILE_PATH);

	if (IsEnableDebugMessage == 1) {
		spdlog::set_level(spdlog::level::debug);
		logger::info("Enable Debug Message!");
	}

	return true;
}
