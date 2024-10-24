#pragma once
#ifndef FILEDIALOG_H
#define FILEDIALOG_H


#include <windows.h>
#include <commdlg.h>
#include <string>
class FileDialog
{
public:
    
    static std::wstring openFile(const std::wstring& filter = L"Comic Files (*.zip, *.cbz, *.rar)\0*.zip;*.cbz;*.rar\0All Files (*.*)\0*.*\0");
};


#endif // FILEDIALOG_H