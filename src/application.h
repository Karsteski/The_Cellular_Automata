#pragma once

#include <vector>
#include <bitset>
#include <string>

#include "imgui.h"

namespace KSS
{

	void CallMenu();

	void CallBasicDrawingGrid();

	void Call1DCellularAutomata();

	void Call2DCellularAutomata();

	void CallGameOfLife();

	static void HelpMarker(std::string);

}

