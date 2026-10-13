#pragma once

namespace gradient {
	void draw_tools(glm::vec4 toolbar) {
		ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
		ImGui::SetNextWindowPos(ImVec2(toolbar.x, toolbar.y));
		ImGui::SetNextWindowSize(ImVec2(toolbar.z, toolbar.w));
		ImGui::Begin("Tools");

		ImGui::SameLine();
		ImGui::End();

	}
}