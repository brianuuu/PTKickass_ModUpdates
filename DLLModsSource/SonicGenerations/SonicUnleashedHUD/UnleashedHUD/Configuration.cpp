#include "Configuration.h"
bool Configuration::windowTitle = true;
Configuration::WindowTitleIcon Configuration::windowTitleIcon = WindowTitleIcon::X360;
Configuration::WindowTitleRegion Configuration::windowTitleRegion = WindowTitleRegion::English;

bool Configuration::unleashedResultMusic = true;

Configuration::ButtonType Configuration::buttonType = ButtonType::X360;
Configuration::SonicType Configuration::uiType = SonicType::Hedgehog;
bool Configuration::checkpointSpeed = true;
bool Configuration::showTime = true;
bool Configuration::showBoost = true;

void Configuration::Read()
{
	INIReader reader(INI_FILE);

	// Appearance
	windowTitle		  = reader.GetBoolean("Appearance", "windowTitle", windowTitle);
	windowTitleIcon   = (WindowTitleIcon)reader.GetInteger("Appearance", "windowTitleIcon", (int)windowTitleIcon);
	windowTitleRegion = (WindowTitleRegion)reader.GetInteger("Appearance", "windowTitleRegion", windowTitleRegion);

	// Music
	unleashedResultMusic = reader.GetBoolean("Music", "resultMusic", unleashedResultMusic);

	// HUD
	buttonType = (ButtonType)reader.GetInteger("HUD", "buttonType", (int)buttonType);
	uiType = (SonicType)reader.GetInteger("HUD", "uiType", (int)uiType);
	checkpointSpeed = reader.GetBoolean("HUD", "checkpointSpeed", checkpointSpeed);
	showTime = reader.GetBoolean("HUD", "showTime", showTime);
	showBoost = reader.GetBoolean("HUD", "showBoost", showBoost);
}
