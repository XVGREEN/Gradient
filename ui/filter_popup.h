#pragma once
#include <vector>
#include <string>
#include <cstring>

// Needs: ImGui, and gradient::program / strength / red / green / blue (core.h),
//        gradient::shaders::*_program (programs.h). C++17 (inline variables).

namespace gradient {
    namespace ui {

        struct Param {
            const char* label;
            float* value;   // points at the global the shader reads (strength, red, ...)
            float       min, max, def;
        };

        struct FilterDef {
            const char* name;
            const char* group;   // submenu it appears in under Filters
            unsigned int* prog;    // pointer, because the program ids are assigned in initPrograms()
            std::vector<Param>  params;  // empty = applies instantly, no popup
        };

        // ---------- registry ----------
        inline std::vector<FilterDef>& filters() {
            using namespace gradient::shaders;
            auto S = [](float mn, float mx, float df) {
                return std::vector<Param>{ { "Strength", & gradient::strength, mn, mx, df } };
                };
            static std::vector<FilterDef> list = {
                { "Vignete",          "Lens", &vignete_program,   S(0.f, 5.f, 0.5f) },
                { "Adjust color",     "Color", &color_program,     { { "Red",   &gradient::red,   0.f, 255.f, 0.f },
                                                                { "Green", &gradient::green, 0.f, 255.f, 0.f },
                                                                { "Blue",  &gradient::blue,  0.f, 255.f, 0.f } } },
                { "Contrast",         "Color", &contrast_program,  S(0.f, 5.f, 0.5f) },
                { "Chromatic",        "Lens", &chromatic_program, S(0.f, 5.f, 1.f) },
                { "Radial chromatic", "Lens", &radial_program,    {} },
                { "Grayscale",        "Color", &gray_program,      {} },
                { "Gaussian blur",    "Detail", &gausian_program,   S(0.f, 5.f, 1.f) },
                { "Matrix",           "Color", &matrix_program,    {} },
                { "Brightness", "Color", &brightness_program, S(-1.f, 1.f, 0.3f) },
                { "Saturation", "Color", &saturation_program, S(-1.f, 1.f, 0.5f) },
                { "Hue shift",  "Color", &hue_shift_program,  S(0.f, 1.f, 0.25f) },
                { "S-curve",    "Color", &s_curve_program,    S(0.f, 1.f, 0.5f) },
                { "Invert",     "Color", &invert_program,     {} },
                { "Sepia",      "Color", &sepia_program,      {} },
                { "Duotone",    "Color", &duotone_program,    {} },
                { "Threshold",  "Color", &threshold_program,  S(0.f, 1.f, 0.5f) },
                { "Posterize",  "Color", &posterize_program,  S(0.f, 1.f, 0.3f) },
                { "Pixelate", "Distort", &pixelate_program, S(0.f, 1.f, 0.3f) },
                { "Wave",     "Distort", &wave_program,     S(0.f, 1.f, 0.5f) },
                { "Barrel",   "Lens", &barrel_program,   S(-1.f, 1.f, 0.5f) },
                { "Swirl",    "Distort", &swirl_program,    S(0.f, 1.f, 0.5f) },
                { "Mirror",   "Distort", &mirror_program,   {} },
                { "Glitch",   "Distort", &glitch_program,   S(0.f, 1.f, 0.5f) },
                { "Turbulence", "Distort", &turbulence_program, S(0.f, 1.f, 0.25f) },
                { "Sharpen",     "Detail", &sharpen_program,     S(0.f, 1.f, 0.5f) },
                { "Edge detect", "Detail", &edge_detect_program, {} },
                { "Emboss",      "Detail", &emboss_program,      S(0.f, 1.f, 0.5f) },
                { "Bloom",      "Stylize", &bloom_program,     S(0.f, 1.f, 0.5f) },
                { "Film grain", "Stylize", &film_grain_program, S(0.f, 1.f, 0.3f) },
                { "Halftone",   "Stylize", &halftone_program,  {} },
            };
            return list;
        }

        // ---------- popup state ----------
        inline const FilterDef* open_filter = nullptr;   // filter whose popup is showing
        inline bool                has_snapshot = false;     // state from before the popup opened
        inline unsigned int        snap_program = 0;
        inline std::vector<float>  snap_values;

        inline void resetParams(const FilterDef& f) {
            for (const Param& p : f.params) *p.value = p.def;
        }

        inline void takeSnapshot(const FilterDef& f) {
            snap_program = gradient::program;
            snap_values.clear();
            for (const Param& p : f.params) snap_values.push_back(*p.value);
            has_snapshot = true;
        }

        inline void cancelFilter() {
            if (open_filter && has_snapshot) {
                gradient::program = snap_program;
                for (size_t i = 0; i < open_filter->params.size() && i < snap_values.size(); i++)
                    *open_filter->params[i].value = snap_values[i];
            }
            open_filter = nullptr;
            has_snapshot = false;
        }

        inline void applyFilter() {
            open_filter = nullptr;
            has_snapshot = false;
        }

        // called from the menu
        inline void selectFilter(const FilterDef& f) {
            if (f.params.empty()) {
                // no parameters: apply instantly, close any open popup
                applyFilter();
                gradient::program = *f.prog;
                return;
            }
            if (!has_snapshot) {
                // remember what was active before the first popup so Cancel can restore it
                // (the snapshot of the previous program only, values are of this filter's params)
                takeSnapshot(f);
            }
            else {
                // switching between popups: keep the original program, refresh the values snapshot
                unsigned int keep = snap_program;
                takeSnapshot(f);
                snap_program = keep;
            }
            gradient::program = *f.prog;
            resetParams(f);
            open_filter = &f;
        }

        // ---------- menu ----------
        inline void drawFiltersMenu() {
            static const char* groups[] = { "Color", "Lens", "Distort", "Detail", "Stylize" };
            auto& list = filters();

            for (const char* g : groups) {
                if (!ImGui::BeginMenu(g)) continue;
                for (const FilterDef& f : list) {
                    if (std::strcmp(f.group, g) != 0) continue;
                    if (ImGui::MenuItem(f.name, nullptr, gradient::program == *f.prog)) selectFilter(f);
                }
                ImGui::EndMenu();
            }
        }

        // ---------- the popup window (call once per frame, outside any menu) ----------
        inline void drawFilterPopup() {
            if (!open_filter) return;
            const FilterDef& f = *open_filter;

            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            // "###" keeps the window id fixed while the visible title changes per filter
            std::string title = std::string(f.name) + "###filter_popup";
            bool open = true;

            if (ImGui::Begin(title.c_str(), &open,
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
                for (const Param& p : f.params)
                    ImGui::SliderFloat(p.label, p.value, p.min, p.max);

                ImGui::Separator();
                if (ImGui::Button("Reset"))  resetParams(f);
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) open = false;
                ImGui::SameLine();
                if (ImGui::Button("Apply")) { ImGui::End(); applyFilter(); return; }
            }
            ImGui::End();

            if (!open) cancelFilter();   
        }
    }
}