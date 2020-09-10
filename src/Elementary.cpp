#include "Elementary.h"

#include <iostream>

Elementary::Elementary() : m_cellMap(), m_ruleset("00000000"), m_numberOfCellsPerGeneration(32), m_numberOfGenerations(100) { };

std::map<ImVec2, cellState>& Elementary::GetCellMap()
{
	return m_cellMap;
}

void Elementary::GenerateCells(cellState state = inactive)
{
	for (unsigned int generation = 0; generation < m_numberOfGenerations; ++generation)
	{
		for (unsigned int position = 0; position < m_numberOfCellsPerGeneration; ++position)
		{
			ImVec2 cell = ImVec2(position, generation);

			m_cellMap.insert(std::make_pair(cell, state));

			std::cout << "(" << position << ", " << generation << ") - " << state << std::endl;

		}
	}	
}

void Elementary::SetSingleCellState(ImVec2 cell, cellState state)
{
	auto& target_cell = m_cellMap.at(cell);

	target_cell = state;
}

cellState Elementary::GetCellState(ImVec2 cell)
{
	auto iterator = m_cellMap.find(cell);
	cellState state = iterator->second;
	return state;
}

void Elementary::SetAllCellStates()
{
	GenerateCells();
	unsigned int startPoint = m_numberOfCellsPerGeneration / 2;
	SetSingleCellState(ImVec2(startPoint, 0), active);

}

void Elementary::draw_cells() const
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	auto map = m_cellMap;

	for (auto cell : m_cellMap)
	{
		if (cell.second == active)
		{
			ImVec2 cell_pos_i = ImVec2(m_min_canvas_position.x + (cell.first.x * m_grid_steps), m_min_canvas_position.y + (cell.first.y * m_grid_steps));
			ImVec2 cell_pos_f = ImVec2(cell_pos_i.x + m_grid_steps, cell_pos_i.y + m_grid_steps);
			draw_list->AddRectFilled(cell_pos_i, cell_pos_f, m_cell_colour_main);
		}
	}
}

unsigned int Elementary::GetNumberOfGenerations()
{
	return 0;
}

void Elementary::SetNumberOfGenerations(unsigned int number)
{

}

void Elementary::SetRandomRuleset()
{

}

Elementary::~Elementary()
{

}