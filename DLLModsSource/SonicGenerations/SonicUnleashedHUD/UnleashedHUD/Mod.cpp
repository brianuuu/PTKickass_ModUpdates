extern "C" __declspec(dllexport) void Init(const char* path)
{
	if (GetModuleHandle(TEXT("ChipReturns.dll")))
	{
		MessageBox(nullptr, TEXT("'Chip Returns' mod must be higher priority than 'Sonic Unleashed HUD'!"), TEXT("Sonic Unleashed HUD"), MB_ICONERROR);
		exit(-1);
	}

	Configuration::Read();

	ArchiveTreePatcher::Install();
	HudSonicStage::Install();
	HudResult::Install();
	Patches::Install();
}