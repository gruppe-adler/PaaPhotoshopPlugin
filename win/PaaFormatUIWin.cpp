#include "PIDefines.h"
#include "PIFormat.h"
#include "PIUtilities.h"
#include "PIUI.h"

#include <memory>

#define NOMINMAX
#include <Windows.h>

#include "..\win\AboutDialog.h"

bool DoAboutUI(FormatRecordPtr gFormatRecord)
{
	AboutRecordPtr aboutRecord = reinterpret_cast<AboutRecordPtr>(gFormatRecord);
	sSPBasic = aboutRecord->sSPBasic;
	auto gPluginRef = static_cast<SPPluginRef>(aboutRecord->plugInRef);
	auto aDiag = std::make_shared<AboutDialog>();
	aDiag->RunModal(GetDLLInstance(gPluginRef), IDD_ABOUT_DIALOG, GetActiveWindow());
	return true;
}


void DoMessageUI(std::string titel, std::string message) {
	MessageBox(GetActiveWindow(), titel.c_str(), message.c_str(), MB_OK | MB_ICONSTOP);
}
