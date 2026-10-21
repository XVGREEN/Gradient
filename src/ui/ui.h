#pragma once
#pragma comment(lib, "comdlg32.lib") 
#include "lowbar.h"
#include "filter_popup.h"
#include "mainmenu.h"
#include "options.h"
#include "tools.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <string>


namespace gradient {
    namespace ui {
        using vec4 = glm::vec4;
        using vec2 = glm::vec2;
        void drawui(vec4 tb, vec4 opt, vec4 lowb, vec4 vp, vec2 mouse) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            drawmenu();
            draw_tools(tb);
            draw_options(opt);
            draw_lowbar(lowb, vp, mouse.x, mouse.y);
            drawFilterPopup();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }
}