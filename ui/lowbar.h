#pragma once
namespace gradient {
	void draw_lowbar(glm::vec4 bottom,glm::vec4 main_viewport,int mousex,int mousey) {

        //info bar 
        ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
        ImGui::SetNextWindowPos(ImVec2(bottom.x, bottom.y));
        ImGui::SetNextWindowSize(ImVec2(bottom.z, bottom.w));
        ImGui::Begin("Info");
        ImGui::Text("Tool:Pen  |  X:%d Y:%d", (int)mousex - (int)main_viewport.x, (int)mousey - (int)main_viewport.y);
        ImGui::SameLine();
        ImGui::Text((std::string("     Preview Mode:") + std::string((filter_preview) ? "yes" : "no")).c_str());
        ImGui::SameLine();
        ImGui::Indent(resolution.x * .35);
        ImGui::SetNextItemWidth(resolution.x * .1);
        ImGui::SliderFloat("Zoom:", &ZOOM, .0, 5.0);
        ImGui::SameLine();
        ImGui::Indent(300.);
        ImGui::SetNextItemWidth(resolution.x * .1);
        ImGui::SliderFloat("Rotation:", &rotation, .0, PI);
        ImGui::End();

	}
}