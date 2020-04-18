#pragma once

#include "paaformat-sym.h"

#include <Windows.h>
#include <CommCtrl.h>

class AboutDialog
{
    static HINSTANCE resModule;

public:
    int RunModal(HINSTANCE resModule, UINT resId, HWND parent) noexcept;
protected:
    static INT_PTR CALLBACK AboutDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

