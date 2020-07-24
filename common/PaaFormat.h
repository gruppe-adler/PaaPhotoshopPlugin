#pragma once

#include "PIDefines.h"
#include "PIFormat.h"
#include "PIUtilities.h"
#include "PIUI.h"
#include "FileUtilities.h"
#include "PaaFormatTerminology.h"
#include <vector>
#include <sstream>
#include <time.h>
#include <string>
#include <vector>

#if __PIMac__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <stdio.h>
#endif

#ifdef _WIN32
    #include <Windows.h>
    #include "..\win\paaformat-sym.h"
#endif

extern SPPluginRef gPluginRef;
extern FormatRecordPtr gFormatRecord;
extern int16* gResult;

// Filter
static void DoFilterFile();

// File IO
static void Read(int32_t count, void* buffer);
static void Write(int32_t count, void* buffer);
static void FromStart();

// Read
static void DoReadPrepare();
static void DoReadStart();
static void DoReadContinue();
static void DoReadFinish();

// Write
static void DoWritePrepare();
static void DoWriteStart();
static void DoWriteContinue();
static void DoWriteFinish();

// Other
static void DoOptionsPrepare();
static void DoOptionsStart();
static void DoEstimatePrepare();
static void DoEstimateStart();
static void DoEstimateContinue();
static void DoEstimateFinish();

static bool isPowerOfTwo(uint32_t x);
static int32_t GetFileSizeFFR();

// UI
bool DoAboutUI(FormatRecordPtr gFormatRecord);
void DoMessageUI(std::string titel, std::string message);
