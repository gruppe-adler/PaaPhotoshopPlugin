#include "PaaFormat.h"

#define GRAD_AFF_STATIC_DEFINE
#include <grad_aff/paa/paa.h>

DLLExport MACPASCAL void PluginMain (const int16 selector, FormatRecordPtr formatParamBlock, intptr_t* data, int16* result);

// Globals
SPBasicSuite* sSPBasic = NULL;
SPPluginRef gPluginRef = NULL;

FormatRecordPtr gFormatRecord = NULL;
int16* gResult = NULL;

DLLExport MACPASCAL void PluginMain(const int16 selector,
	FormatRecordPtr formatParamBlock,
	intptr_t* data,
	int16* result)
{
	try
	{
		// init global
		gFormatRecord = formatParamBlock;
		gPluginRef = reinterpret_cast<SPPluginRef>(gFormatRecord->plugInRef);
		gResult = result;

		if (selector == formatSelectorAbout)
		{
			//MessageBox(GetActiveWindow(), std::to_string(OMP_DYNAMIC).c_str(), "Hi", MB_OK);
			AboutRecordPtr aboutRecord = reinterpret_cast<AboutRecordPtr>(formatParamBlock);
			sSPBasic = aboutRecord->sSPBasic;
			gPluginRef = reinterpret_cast<SPPluginRef>(aboutRecord->plugInRef);
			auto aDiag = new AboutDialog();
			aDiag->RunModal(GetDLLInstance(gPluginRef), IDD_ABOUT_DIALOG, GetActiveWindow());

		}
		else
		{
			sSPBasic = formatParamBlock->sSPBasic;
			if (gFormatRecord->advanceState == NULL) {
				*gResult = errPlugInHostInsufficient;
				return;
			}

			if (gFormatRecord->HostSupports32BitCoordinates)
				gFormatRecord->PluginUsing32BitCoordinates = true;


			switch (selector)
			{

			case formatSelectorReadPrepare:
				DoReadPrepare();
				break;
			case formatSelectorReadStart:
				DoReadStart();
				break;
			case formatSelectorReadContinue:
				DoReadContinue();
				break;
			case formatSelectorReadFinish:
				DoReadFinish();
				break;

			case formatSelectorOptionsPrepare:
				DoOptionsPrepare();
				break;
			case formatSelectorOptionsStart:
				DoOptionsStart();
				break;
			case formatSelectorOptionsContinue:
				break;
			case formatSelectorOptionsFinish:
				break;

			case formatSelectorEstimatePrepare:
				DoEstimatePrepare();
				break;
			case formatSelectorEstimateStart:
				DoEstimateStart();
				break;
			case formatSelectorEstimateContinue:
				DoEstimateContinue();
				break;
			case formatSelectorEstimateFinish:
				DoEstimateFinish();
				break;


			case formatSelectorWritePrepare:
				DoWritePrepare();
				break;
			case formatSelectorWriteStart:
				DoWriteStart();
				break;
			case formatSelectorWriteContinue:
				DoWriteContinue();
				break;
			case formatSelectorWriteFinish:
				DoWriteFinish();
				break;

			case formatSelectorFilterFile:
				DoFilterFile();
				break;
			}
		}

		// release any suites that we may have acquired
		if (selector == formatSelectorAbout ||
			selector == formatSelectorWriteFinish ||
			selector == formatSelectorReadFinish ||
			selector == formatSelectorOptionsFinish ||
			selector == formatSelectorEstimateFinish ||
			selector == formatSelectorFilterFile ||
			*gResult != noErr)
		{
			PIUSuitesRelease();
		}

	}

	catch (...)
	{
		if (NULL != result)
			*result = -1;
	}

}

static void DoFilterFile() {
	return;
}

static void DoReadPrepare() noexcept {
	gFormatRecord->maxData = 0;
}

static void DoReadStart() {
	FromStart();

	LARGE_INTEGER fileSize;
	bool ret = GetFileSizeEx((HANDLE)gFormatRecord->dataFork, &fileSize);
	if (!ret) {
		MessageBox(GetActiveWindow(), (std::string("Error during saving! Code: ") + std::to_string(GetLastError())).c_str(), "PAA Save Error!", MB_OK | MB_ICONSTOP);
	}

	std::vector<uint8_t> fileData;
	fileData.resize(fileSize.QuadPart);

	Read(fileSize.QuadPart, fileData.data());

	auto paa = grad_aff::Paa(fileData);
	paa.readPaa(true);

	auto width = paa.mipMaps[0].width;
	auto height = paa.mipMaps[0].height;

	// correct width if needed
	if ((width & 0x8000) != 0) {
		width &= 0x7FFF;
	}

	gFormatRecord->imageMode = plugInModeRGBColor;

	gFormatRecord->imageSize.h = gFormatRecord->imageSize32.h = width;
	gFormatRecord->imageSize.v = gFormatRecord->imageSize32.v = height;

	gFormatRecord->depth = 8;

	// check for transparency flag
	bool hasAlpha = false;

	for (auto& tagg : paa.taggs) {
		if (tagg.signature == "GGATGALF") {
			hasAlpha = true;
		}
	}

	if (hasAlpha) {
		gFormatRecord->planes = 4;
		gFormatRecord->transparencyPlane = 3;
	}
	else {
		gFormatRecord->planes = 3;
		gFormatRecord->transparencyPlane = -1;
	}

	return;
}

static void DoReadContinue() {
	FromStart();

	LARGE_INTEGER fileSize;
	bool ret = GetFileSizeEx((HANDLE)gFormatRecord->dataFork, &fileSize);
	if (!ret) {
		MessageBox(GetActiveWindow(), (std::string("Error during saving! Code: ") + std::to_string(GetLastError())).c_str(), "PAA Save Error!", MB_OK | MB_ICONSTOP);
	}

	std::vector<uint8_t> fileData;
	fileData.resize(fileSize.QuadPart);

	Read(fileSize.QuadPart, fileData.data());

	auto paa = grad_aff::Paa(fileData);
	paa.readPaa();

	auto width = paa.mipMaps[0].width;
	auto height = paa.mipMaps[0].height;

	gFormatRecord->planeBytes = 1;
	gFormatRecord->colBytes = gFormatRecord->planeBytes * gFormatRecord->planes;
	gFormatRecord->rowBytes = gFormatRecord->colBytes * width;

	gFormatRecord->loPlane = 0;
	gFormatRecord->hiPlane = gFormatRecord->planes - 1;

	gFormatRecord->theRect.left = gFormatRecord->theRect32.left = 0;
	gFormatRecord->theRect.right = gFormatRecord->theRect32.right = width;

	auto data = paa.mipMaps[0].data;

	if (gFormatRecord->planes == 3) {
		auto temp = std::vector<uint8_t>();
		temp.reserve((size_t)width * (size_t)height * 3);
		for (size_t i = 0; i < data.size(); i += 4) {
			temp.push_back(data[i]);
			temp.push_back(data[i + 1]);
			temp.push_back(data[i + 2]);
		}
		data.clear();
		data = temp;
	}

	gFormatRecord->data = data.data();

	gFormatRecord->theRect.top = gFormatRecord->theRect32.top = 0;
	gFormatRecord->theRect.bottom = gFormatRecord->theRect32.bottom = height;

	gFormatRecord->advanceState();

	gFormatRecord->data = NULL;
}

static void DoReadFinish() {
}

static void DoOptionsPrepare() {
	gFormatRecord->maxData = 0;
}

static void DoOptionsStart() {
	gFormatRecord->data = NULL;
}

static void DoEstimatePrepare() {
	gFormatRecord->maxData = 0;
}

static void DoEstimateStart() {
	gFormatRecord->minDataBytes = gFormatRecord->imageSize32.h * gFormatRecord->imageSize32.v * 8;
	gFormatRecord->maxDataBytes = gFormatRecord->imageSize32.h * gFormatRecord->imageSize32.v * 8;
	gFormatRecord->data = NULL;

}

static void DoEstimateContinue() {
}

static void DoEstimateFinish() {
}

static void DoWritePrepare() {
	gFormatRecord->maxData = 0;
}

static void DoWriteStart() {

	if (gFormatRecord->HostSupports32BitCoordinates && gFormatRecord->imageSize32.h && gFormatRecord->imageSize32.v)
		gFormatRecord->PluginUsing32BitCoordinates = TRUE;

	const int width = (gFormatRecord->PluginUsing32BitCoordinates ? gFormatRecord->imageSize32.h : gFormatRecord->imageSize.h);
	const int height = (gFormatRecord->PluginUsing32BitCoordinates ? gFormatRecord->imageSize32.v : gFormatRecord->imageSize.v);

	if (!isPowerOfTwo(width) || !isPowerOfTwo(height)) {
		MessageBox(GetActiveWindow(), "Dimensions have to be a power of two (2^n)", "PAA save error!", MB_OK | MB_ICONSTOP);
		return;
	}

	if (width != height && width / 2 != height && width != height / 2) {
		MessageBox(GetActiveWindow(), "Aspect ratio has to be 1:1, 1:2 or 2:1", "PAA save error!", MB_OK | MB_ICONSTOP);
		return;
	}

	if (gFormatRecord->imageMode != plugInModeRGBColor) {
		MessageBox(GetActiveWindow(), "Currently only RGB Mode is supported", "PAA save error!", MB_OK | MB_ICONSTOP);
		return;
	}

	bool hasTransparency = (gFormatRecord->planes == 4);

	gFormatRecord->depth = 8;

	gFormatRecord->planes = hasTransparency ? 4 : 3;
	gFormatRecord->loPlane = 0;
	gFormatRecord->hiPlane = gFormatRecord->planes - 1;

	gFormatRecord->planeBytes = 1;
	gFormatRecord->colBytes = gFormatRecord->planeBytes * gFormatRecord->planes;
	gFormatRecord->rowBytes = gFormatRecord->colBytes * width;

	gFormatRecord->theRect.left = gFormatRecord->theRect32.left = 0;
	gFormatRecord->theRect.right = gFormatRecord->theRect32.right = width;

	std::vector<uint8_t> data((size_t)height * (size_t)width * gFormatRecord->planes);

	gFormatRecord->data = data.data();

	gFormatRecord->theRect.top = gFormatRecord->theRect32.top = 0;
	gFormatRecord->theRect.bottom = gFormatRecord->theRect32.bottom = height;

	gFormatRecord->advanceState();

	if (!hasTransparency) {
		auto temp = std::vector<uint8_t>();
		temp.reserve((size_t)width * (size_t)height * 4);
		for (size_t i = 0; i < data.size(); i += 3) {
			temp.push_back(data[i]);
			temp.push_back(data[i + 1]);
			temp.push_back(data[i + 2]);
			temp.push_back(255);
		}
		data.clear();
		data = temp;
	}

	auto paa = grad_aff::Paa();
	paa.mipMaps.clear();

	paa.hasTransparency = hasTransparency;
	MipMap mipMap;
	mipMap.height = height;
	mipMap.width = width;
	mipMap.data = data;
	mipMap.dataLength = mipMap.data.size();

	paa.mipMaps.clear();
	paa.mipMaps.push_back(mipMap);
	paa.calculateMipmapsAndTaggs();

	auto dataStream = std::stringstream(std::string());
	paa.writePaa(dataStream);

	auto output = dataStream.str();
	auto dataBuf = std::vector<uint8_t>(output.begin(), output.end());

	Write(dataBuf.size(), dataBuf.data());
	auto err = GetLastError();
	gFormatRecord->data = NULL;

	if (err != 0) {
		MessageBox(GetActiveWindow(), (std::string("Error during saving! Code: ") + std::to_string(err)).c_str(), "PAA save error!", MB_OK | MB_ICONSTOP);
	}
}

static void DoWriteContinue() {

}

static void DoWriteFinish() {

}

static bool isPowerOfTwo(uint32_t x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

static void FromStart() {
	auto result = PSSDKSetFPos(gFormatRecord->dataFork, gFormatRecord->posixFileDescriptor, gFormatRecord->pluginUsingPOSIXIO, fsFromStart, 0);

	*gResult = result;
	if (result != noErr) {
		MessageBox(GetActiveWindow(), (std::string("Error! Code: ") + std::to_string(GetLastError())).c_str(), "PAA Save Error!", MB_OK | MB_ICONSTOP);
	}
}

static void Read(int32_t count, void* buffer) {
	auto readCount = count;
	auto result = PSSDKRead(gFormatRecord->dataFork, gFormatRecord->posixFileDescriptor, gFormatRecord->pluginUsingPOSIXIO, &readCount, buffer);

	*gResult = result;
	if (result == noErr && readCount != count) {
		MessageBox(GetActiveWindow(), "Disk is Full!", "PAA save error!", MB_OK | MB_ICONSTOP);
		*gResult = eofErr;
	}
	else if (result != noErr) {
		MessageBox(GetActiveWindow(), (std::string("Error during reading! Code: ") + std::to_string(GetLastError())).c_str(), "PAA read error!", MB_OK | MB_ICONSTOP);
	}
}

static void Write(int32_t count, void* buffer) {
	auto writeCount = count;
	auto result = PSSDKWrite(gFormatRecord->dataFork, gFormatRecord->posixFileDescriptor, gFormatRecord->pluginUsingPOSIXIO, &writeCount, buffer);

	*gResult = result;
	if (result == noErr && writeCount != count) {
		MessageBox(GetActiveWindow(), "Disk is Full!", "PAA save error!", MB_OK | MB_ICONSTOP);
		*gResult = eofErr;
	}
	else if (result != noErr) {
		MessageBox(GetActiveWindow(), (std::string("Error during writing! Code: ") + std::to_string(GetLastError())).c_str(), "PAA write error!", MB_OK | MB_ICONSTOP);
	}
}