#pragma once
#include "../core/open_file_dialog.h"
namespace gradient {
    namespace ui {

        
        inline void filterItem(const char* label, unsigned int prog, float defaultStrength = 0.f) {
            if (ImGui::MenuItem(label, nullptr, program == prog)) {
                program = prog;
                strength = defaultStrength;
            }
        }

        inline void drawmenu() {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New", "Ctrl+N")) {
                    }
                    if (ImGui::MenuItem("Open", "Ctrl+O")) {
                        gradient::core::open_requested = true;
                    }
                    if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    }
                    if (ImGui::MenuItem("Save as..")) {
                    }
                    if (ImGui::MenuItem("Exit")) {
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit")) {
                    if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                    }
                    if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View")) {
                    if (ImGui::MenuItem("Filter Previews")) {
                    }
                    if (ImGui::MenuItem("Grid")) {
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Filters")) {
                    drawFiltersMenu();
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Settings")) {
                    if (ImGui::MenuItem("Filter Preview Mode", nullptr, filter_preview)) {
                        filter_preview = !filter_preview;
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Help")) {
                    if (ImGui::MenuItem("Help")) {
                    }
                    if (ImGui::MenuItem("About")) {
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
        }
    }
}