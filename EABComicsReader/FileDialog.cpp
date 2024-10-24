#include "FileDialog.h"

std::wstring FileDialog::openFile(const std::wstring& filter) {
    OPENFILENAME ofn;
    wchar_t szFile[260] = L""; // Buffer for file name, initialized to empty string
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr; // Handle to the owner window (nullptr if no owner)
    ofn.lpstrFile = szFile; // Buffer for file name
    ofn.nMaxFile = sizeof(szFile); // Size of the buffer
    ofn.lpstrFilter = filter.c_str(); // Use the provided filter
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Display the dialog
    if (GetOpenFileName(&ofn)) {
        return std::wstring(szFile); // Return the selected file path as a std::wstring
    }

    return L""; // Return an empty string if canceled
}
