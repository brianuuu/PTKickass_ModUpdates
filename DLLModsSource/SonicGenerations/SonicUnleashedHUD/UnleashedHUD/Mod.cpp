
extern "C" __declspec(dllexport) void Init(ModInfo_t * modInfo)
{
	Configuration::Read();

	ArchiveTreePatcher::Install();
	HudSonicStage::Install();
	HudResult::Install(); // Must be hooked after HudSonicStage to get checkpoint speed data
	HudLoading::Install();
	HudPause::Install();
	Patches::Install();
}

extern "C" __declspec(dllexport) void PostInit(ModInfo_t * modInfo)
{
	auto CheckPriority = [&modInfo](std::string const& dllName, bool isHighPriority = true) -> bool
	{
		auto* api = modInfo->API;
		auto* thisMod = modInfo->CurrentMod;

		std::string modID;
		if (Common::GetModIDFromDLL(dllName, modID))
		{
			auto* otherMod = api->FindMod(modID.c_str());
			if (otherMod)
			{
				// 0 = highest priority
				if (isHighPriority != otherMod->Priority < thisMod->Priority)
				{
					return false;
				}
			}
		}

		return true;
	};

	if (!CheckPriority("ChipReturns.dll"))
	{
		MessageBox(nullptr, TEXT("'Chip Returns' mod must be higher priority than 'Sonic Unleashed HUD'!"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	if (!CheckPriority("ScoreGenerations.dll", false))
	{
		MessageBox(nullptr, TEXT("'Score Generations' mod must be lower priority than 'Sonic Unleashed HUD'!"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	if (Common::IsModEnabled("Main", "DLLFile", "ERankGenerations.dll"))
	{
		MessageBox(nullptr, TEXT("'E-Rank Generations' mod is not compatible with 'Sonic Unleashed HUD'! Please disable it."), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	if (Configuration::unleashedResultMusic && Common::IsModEnabled("Main", "DLLFile", "CustomizableResultsMusic.dll"))
	{
		MessageBox(nullptr, TEXT("'Customizable Results Music' mod is not compatible with 'Sonic Unleashed HUD' when Unleashed Music is enabled! Either disable Unleashed Music in configuration or the mod"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}
}