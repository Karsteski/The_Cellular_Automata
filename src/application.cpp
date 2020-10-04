#include "Application.h"
#include "Grid.h"




void KSS::CallMenu()
{	
	// WIP
}

void KSS::CallBasicDrawingGrid()
{
	// WIP
}

void KSS::Call1DCellularAutomata()
{
	if (ImGui::BeginTabItem("1D Cellular Automata"))
	{
		// WIP

		ImGui::EndTabItem();
	}
}

void KSS::Call2DCellularAutomata()
{
	if (ImGui::BeginTabItem("2D Cellular Automata"))
	{
		// WIP

		ImGui::EndTabItem();
	}
}

void KSS::CallGameOfLife()
{
	if (ImGui::BeginTabItem("Conway's Game of Life"))
	{
		// WIP

		ImGui::EndTabItem();
	}
}

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