#pragma once
namespace gradient {
    void drawmenu() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    printf("baka");
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
                if (ImGui::MenuItem("Redo", "Ctrl + Y")) {
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
                if (ImGui::MenuItem("Vignete")) {
                    program = vignete_program;
                }
                if (ImGui::MenuItem("adjust color")) {
                    program = color_program;
                    strength = 0;
                }

                if (ImGui::MenuItem("contrast")) {
                    program = contrast_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("Chormatic")) {
                    program = chromatic_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("Radial chormatic")) {
                    program = radial_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("grayscale")) {
                    program = gray_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("gausin")) {
                    program = gausian_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("matrix")) {
                    program = matrix_program;
                    strength = 0;
                }
                ImGui::EndMenu();

            }

            if (ImGui::BeginMenu("Settings")) {
                if (ImGui::MenuItem("Filter Preview Mode")) {
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