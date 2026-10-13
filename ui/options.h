#pragma once

namespace gradient {
    void draw_options(glm::vec4 options_bar) {
        //options window
        ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
        ImGui::SetNextWindowPos(ImVec2(options_bar.x, options_bar.y));
        ImGui::SetNextWindowSize(ImVec2(options_bar.z, options_bar.w));
        ImGui::Begin("Options");
        if (gradient::program == gradient::vignete_program || gradient::program == gradient::contrast_program || gradient::program == gradient::chromatic_program || gradient::program == gradient::gausian_program) {
            ImGui::SliderFloat("Strength", &gradient::strength, 0., 5.);
        }
        if (gradient::program == gradient::color_program) {
            ImGui::SliderFloat("Red", &gradient::red, 0., 255.);
            ImGui::SliderFloat("Green", &gradient::green, 0., 255.);
            ImGui::SliderFloat("Blue", &gradient::blue, 0., 255.);
        }

        ImGui::End();
    }
}