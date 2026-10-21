#pragma once
#include <vector>
#include <cstdio>
#include <cstring>
#include <utility>
#include <algorithm>

namespace gradient {

    struct Layer {
        char  name[64] = "";
        bool  visible = true;
        bool  locked = false;
        float opacity = 100.f;
        int   blend = 0;

        Layer(const char* n = "Layer") { std::snprintf(name, sizeof(name), "%s", n); }
    };

    struct Project {
        char  name[64] = "Untitled";
        int   width = 1920;
        int   height = 1080;
        int   dpi = 72;
        int   background = 0;
        float background_color[3] = { 1.f, 1.f, 1.f };
        bool  show_grid = false;
        int   grid_size = 32;
    };

    inline Project project;
    inline std::vector<Layer> layers = { Layer("Background") };
    inline int active_layer = 0;
    inline int layer_counter = 1;

    namespace ui {

        inline void draw_project_settings() {
            if (!ImGui::CollapsingHeader("Project", ImGuiTreeNodeFlags_DefaultOpen)) return;

            static const char* backgrounds[] = { "White", "Black", "Transparent", "Custom" };

            ImGui::InputText("Name##project", project.name, sizeof(project.name));

            ImGui::InputInt("Width", &project.width);
            ImGui::InputInt("Height", &project.height);
            ImGui::InputInt("DPI", &project.dpi);
            project.width = std::max(1, project.width);
            project.height = std::max(1, project.height);
            project.dpi = std::max(1, project.dpi);

            ImGui::Combo("Background", &project.background, backgrounds, IM_ARRAYSIZE(backgrounds));
            if (project.background == 3) {
                ImGui::ColorEdit3("Color", project.background_color);
            }

            ImGui::Checkbox("Show grid", &project.show_grid);
            if (project.show_grid) {
                ImGui::SliderInt("Grid size", &project.grid_size, 4, 256);
            }
        }

        inline void draw_layers() {
            if (!ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen)) return;

            static const char* blend_modes[] = {
                "Normal", "Multiply", "Screen", "Overlay", "Darken", "Lighten", "Add"
            };

            const int count = (int)layers.size();
            active_layer = std::clamp(active_layer, 0, count - 1);

            ImGui::BeginChild("##layer_list", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 6), true);
            for (int i = count - 1; i >= 0; --i) {
                Layer& l = layers[i];
                ImGui::PushID(i);

                ImGui::Checkbox("##visible", &l.visible);
                ImGui::SameLine();

                const float lock_w = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.x;
                const float name_w = ImGui::GetContentRegionAvail().x - lock_w;

                if (!l.visible) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
                if (ImGui::Selectable(l.name, active_layer == i, 0, ImVec2(name_w, 0))) active_layer = i;
                if (!l.visible) ImGui::PopStyleColor();

                ImGui::SameLine();
                ImGui::Checkbox("##locked", &l.locked);

                ImGui::PopID();
            }
            ImGui::EndChild();

            if (ImGui::Button("Add")) {
                char buf[64];
                std::snprintf(buf, sizeof(buf), "Layer %d", ++layer_counter);
                layers.insert(layers.begin() + active_layer + 1, Layer(buf));
                active_layer++;
            }
            ImGui::SameLine();
            if (ImGui::Button("Duplicate")) {
                Layer copy = layers[active_layer];
                char buf[64];
                std::snprintf(buf, sizeof(buf), "%.55s copy", copy.name);
                std::snprintf(copy.name, sizeof(copy.name), "%s", buf);
                layers.insert(layers.begin() + active_layer + 1, copy);
                active_layer++;
            }
            ImGui::SameLine();
            ImGui::BeginDisabled(layers.size() <= 1);
            if (ImGui::Button("Delete")) {
                layers.erase(layers.begin() + active_layer);
                active_layer = std::clamp(active_layer, 0, (int)layers.size() - 1);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(active_layer >= (int)layers.size() - 1);
            if (ImGui::Button("Up")) {
                std::swap(layers[active_layer], layers[active_layer + 1]);
                active_layer++;
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::BeginDisabled(active_layer <= 0);
            if (ImGui::Button("Down")) {
                std::swap(layers[active_layer], layers[active_layer - 1]);
                active_layer--;
            }
            ImGui::EndDisabled();

            ImGui::SeparatorText("Layer properties");

            Layer& l = layers[active_layer];
            ImGui::InputText("Name##layer", l.name, sizeof(l.name));
            ImGui::SliderFloat("Opacity", &l.opacity, 0.f, 100.f, "%.0f%%");
            ImGui::Combo("Blend", &l.blend, blend_modes, IM_ARRAYSIZE(blend_modes));
            ImGui::Checkbox("Visible", &l.visible);
            ImGui::SameLine();
            ImGui::Checkbox("Locked", &l.locked);
        }

        inline void draw_options(glm::vec4 options_bar) {
            ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
            ImGui::SetNextWindowPos(ImVec2(options_bar.x, options_bar.y));
            ImGui::SetNextWindowSize(ImVec2(options_bar.z, options_bar.w));
            ImGui::Begin("Options");

            draw_project_settings();
            draw_layers();

            ImGui::End();
        }
    }
}