#pragma once
#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <string>

#pragma comment(lib, "comdlg32.lib")   

namespace gradient {
    namespace core {
        inline bool open_requested = false;   

        inline std::string openFileDialog(HWND owner = nullptr) {
            char file[MAX_PATH] = {};

            OPENFILENAMEA ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = owner;
            ofn.lpstrFile = file;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFilter = "Images\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0All files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

            return GetOpenFileNameA(&ofn) ? std::string(file) : std::string();
        }
    }
}