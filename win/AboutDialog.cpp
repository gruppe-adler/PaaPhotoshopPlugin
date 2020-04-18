#include "AboutDialog.h"

int AboutDialog::RunModal(HINSTANCE resModule, UINT resId, HWND parent) noexcept {
    this->resModule = resModule;
    return DialogBoxParam(resModule, MAKEINTRESOURCE(resId), parent, &AboutDialogProc, (LPARAM)this);
}

INT_PTR CALLBACK AboutDialog::AboutDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_INITDIALOG:
    {
        // Remove Icon
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, NULL);
        // Load Logo
        auto hBitmap = LoadBitmapA(resModule, MAKEINTRESOURCE(IDB_LOGO_BITMAP));
        SendDlgItemMessage(hwnd, IDC_LOGO_CONTROL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        DeleteObject(hBitmap);

        // Center Window
        HWND hwndParent;
        RECT rectWindow, rectParent;

        if ((hwndParent = GetParent(hwnd)) != NULL)
        {
            GetWindowRect(hwnd, &rectWindow);
            GetWindowRect(hwndParent, &rectParent);

            auto nWidth = rectWindow.right - rectWindow.left;
            auto nHeight = rectWindow.bottom - rectWindow.top;

            auto nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
            auto nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

            auto nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
            auto nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

            if (nX < 0) nX = 0;
            if (nY < 0) nY = 0;
            if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
            if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;
            MoveWindow(hwnd, nX, nY, nWidth, nHeight, FALSE);
        }
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    case WM_NOTIFY: {
        switch (wParam)
        {
        case IDC_HP_LINK:
            switch (((LPNMHDR)lParam)->code)
            {
            case NM_CLICK:
            case NM_RETURN:
                ShellExecute(NULL, "open", "https://www.gruppe-adler.de/", NULL, NULL, SW_SHOWNORMAL);
                break;
            }
            break;
        case IDC_GITHUB_LINK:
            switch (((LPNMHDR)lParam)->code)
            {
            case NM_CLICK:
            case NM_RETURN:
                ShellExecute(NULL, "open", "http://github.gruppe-adler.de/", NULL, NULL, SW_SHOWNORMAL);
                break;
            }
            break;
        }
    }
    default:
        return FALSE;
    }
    return TRUE;
}

HINSTANCE AboutDialog::resModule;