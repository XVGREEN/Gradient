#pragma once

#define TOOL_ICON_SIZE     20.0f
#define TOOL_ICON_PADDING  4.0f
#define TOOL_ACTIVE_BG     ImVec4(0.20f, 0.50f, 0.90f, 0.45f)
#define TOOL_HOVER_BG      ImVec4(1.00f, 1.00f, 1.00f, 0.12f)

#include <vector>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <cstring>

#pragma push_macro("STB_IMAGE_IMPLEMENTATION")
#undef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma pop_macro("STB_IMAGE_IMPLEMENTATION")

#ifdef _WIN32
#include <direct.h>
#endif

namespace gradient {

    enum class Tool {
        Select, Move, Zoom, Crop,
        Brush, Eraser, Fill, Eyedropper,
        Line, Rectangle, Ellipse, Text
    };

    inline Tool tool = Tool::Brush;

    namespace ui {

        inline constexpr bool ICON_FLIP_V = false;
        inline constexpr bool ICON_FLIP_U = false;

        struct ToolIcon {
            Tool         value;
            const char* label;
            const char* group;
            const char* file;
            unsigned int tex;
        };

        inline std::vector<ToolIcon>& toolIcons() {
            static std::vector<ToolIcon> icons = {
                { Tool::Select,     "Select",     "Edit",   "icons/select.png",     0 },
                { Tool::Move,       "Move",       "Edit",   "icons/move.png",       0 },
                { Tool::Zoom,       "Zoom",       "Edit",   "icons/zoom.png",       0 },
                { Tool::Crop,       "Crop",       "Edit",   "icons/crop.png",       0 },

                { Tool::Brush,      "Brush",      "Paint",  "icons/brush.png",      0 },
                { Tool::Eraser,     "Eraser",     "Paint",  "icons/eraser.png",     0 },
                { Tool::Fill,       "Fill",       "Paint",  "icons/fill.png",       0 },
                { Tool::Eyedropper, "Eyedropper", "Paint",  "icons/dropper.png", 0 },

                { Tool::Line,       "Line",       "Shapes", "icons/line.png",       0 },
                { Tool::Rectangle,  "Rectangle",  "Shapes", "icons/rectangle.png",  0 },
                { Tool::Ellipse,    "Ellipse",    "Shapes", "icons/elipse.png",    0 },
                { Tool::Text,       "Text",       "Shapes", "icons/Text.png",       0 },
            };
            return icons;
        }

        inline unsigned int loadIconTexture(const char* path) {
            int w = 0, h = 0, ch = 0;
            unsigned char* data = stbi_load(path, &w, &h, &ch, 4);
            if (!data) {
                std::printf("icon not loaded: %s (%s)\n", path, stbi_failure_reason());
                return 0;
            }

            GLuint id = 0;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            stbi_image_free(data);
            return id;
        }

        inline void initToolIcons() {
#ifdef _WIN32
            char cwd[512];
            if (_getcwd(cwd, sizeof(cwd))) std::printf("icons loaded relative to: %s\n", cwd);
#endif
            for (ToolIcon& t : toolIcons()) {
                t.tex = loadIconTexture(t.file);
                if (!t.tex) std::printf("tool icon missing: %s\n", t.file);
            }
        }

        inline void freeToolIcons() {
            for (ToolIcon& t : toolIcons()) {
                if (t.tex) { GLuint id = t.tex; glDeleteTextures(1, &id); t.tex = 0; }
            }
        }

        inline void iconButton(const ToolIcon& t, float size) {
            const bool   active = (tool == t.value);
            const ImVec4 clear(0.f, 0.f, 0.f, 0.f);
            const ImVec4 bg = active ? TOOL_ACTIVE_BG : clear;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(TOOL_ICON_PADDING, TOOL_ICON_PADDING));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
            ImGui::PushStyleColor(ImGuiCol_Button, clear);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, TOOL_HOVER_BG);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, TOOL_HOVER_BG);

            bool clicked;
            if (t.tex) {
                clicked = ImGui::ImageButton(t.label, (ImTextureID)(intptr_t)t.tex,
                    ImVec2(size, size),
                    ImVec2(ICON_FLIP_U ? 1.f : 0.f, ICON_FLIP_V ? 1.f : 0.f),
                    ImVec2(ICON_FLIP_U ? 0.f : 1.f, ICON_FLIP_V ? 0.f : 1.f),
                    bg);
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, active ? TOOL_ACTIVE_BG : clear);
                clicked = ImGui::Button(t.label, ImVec2(size, size));
                ImGui::PopStyleColor();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(2);

            if (clicked) tool = t.value;
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", t.label);
        }

        inline void draw_tools(glm::vec4 toolbar) {
            ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
            ImGui::SetNextWindowPos(ImVec2(toolbar.x, toolbar.y));
            ImGui::SetNextWindowSize(ImVec2(toolbar.z, toolbar.w));
            ImGui::Begin("Tools");

            const float size = TOOL_ICON_SIZE;
            const ImGuiStyle& st = ImGui::GetStyle();
            const float step = size + TOOL_ICON_PADDING * 2.f + st.ItemSpacing.x;
            const int perRow = std::max(1, int(ImGui::GetContentRegionAvail().x / step));

            static const char* groups[] = { "Edit", "Paint", "Shapes" };

            for (int g = 0; g < 3; g++) {
                if (g > 0) ImGui::SeparatorText(groups[g]);

                int n = 0;
                for (const ToolIcon& t : toolIcons()) {
                    if (std::strcmp(t.group, groups[g]) != 0) continue;
                    if (n % perRow != 0) ImGui::SameLine();
                    iconButton(t, size);
                    n++;
                }
            }

            ImGui::End();
        }
    }
}