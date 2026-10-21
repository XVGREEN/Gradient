#pragma once
namespace gradient {

    inline bool  save_as_requested = false;

    inline std::string saveAsFileDialog(HWND owner = nullptr) {
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