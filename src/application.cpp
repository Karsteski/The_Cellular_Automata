#include "Application.h"

static void KSS::HelpMarker(std::string description)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(description.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}