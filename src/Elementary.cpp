#include "Elementary.h"

#include <iostream>
#include <algorithm>

Elementary::Elementary() : m_cellMap(), m_ruleset("00000000"), m_numberOfCellsPerGeneration(8), m_numberOfGenerations(8) { };

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

	for (auto& cell : m_cellMap)
	{
		/*
		if (cell.first.x == 0 or cell.first.y == 0)
		{
			//do nothing
		}
		*/
		//else
		{
			std::bitset<3> previousCells("000");
			
			// Note that bitset reads from right to left, so 0 position is the furthest right bit.
			ImVec2 prev_cell_0 = ImVec2(cell.first.x + 1, cell.first.y - 1);
			ImVec2 prev_cell_1 = ImVec2(cell.first.x, cell.first.y - 1);
			ImVec2 prev_cell_2 = ImVec2(cell.first.x - 1, cell.first.y - 1);

			if (m_cellMap.find(prev_cell_0) != m_cellMap.end())
			{
				cellState final_0 = inactive;
				cellState final_1 = inactive;
				cellState final_2 = inactive;

				if (m_cellMap.find(prev_cell_0) != m_cellMap.end())
				{
					const auto& check_0 = m_cellMap.find(prev_cell_0);
					const auto& value_0 = *check_0;
					final_0 = m_cellMap.at(value_0.first);
				}

				if (m_cellMap.find(prev_cell_1) != m_cellMap.end())
				{
					const auto& check_1 = m_cellMap.find(prev_cell_1);
					const auto& value_1 = *check_1;
					final_1 = m_cellMap.at(value_1.first);
				}

				if (m_cellMap.find(prev_cell_2) != m_cellMap.end())
				{
					const auto& check_2 = m_cellMap.find(prev_cell_2);
					const auto& value_2 = *check_2;
					final_2 = m_cellMap.at(value_2.first);
				}

				if (final_0 == active) previousCells[0].flip();
				if (final_1 == active) previousCells[1].flip();
				if (final_2 == active) previousCells[2].flip();

				auto neighbourCellsState = previousCells.to_string();
				//std::cout << "State" << " - " << neighbourCellsState << std::endl;
				//std::reverse(neighbourCellsState.begin(), neighbourCellsState.end());

				//Elementary Cellular Automata Rules (Rule 90)
				std::string rulePosition_0 = "111";
				std::string rulePosition_1 = "110";
				std::string rulePosition_2 = "101";
				std::string rulePosition_3 = "100";
				std::string rulePosition_4 = "011";
				std::string rulePosition_5 = "010";
				std::string rulePosition_6 = "001";
				std::string rulePosition_7 = "000";


				// This needs some heavy reworking lol.
				const std::map <std::string, int> ruleMap =
				{
					{ rulePosition_0, 0 },
					{ rulePosition_1, 1 },
					{ rulePosition_2, 2 },
					{ rulePosition_3, 3 },
					{ rulePosition_4, 4 },
					{ rulePosition_5, 5 },
					{ rulePosition_6, 6 },
					{ rulePosition_7, 7 }
				};

				switch (ruleMap.at(neighbourCellsState))
				{
				case 0:
				{
					auto& targetCellState = cell.second;
					targetCellState = inactive;
					break;
				}
				case 1:
				{
					auto& targetCellState = cell.second;
					targetCellState = active;
					break;
				}
				case 2:
				{
					auto& targetCellState = cell.second;
					targetCellState = inactive;
					break;
				}
				case 3:
				{
					auto& targetCellState = cell.second;
					targetCellState = active;
					break;
				}
				case 4:
				{
					auto& targetCellState = cell.second;
					targetCellState = active;
					break;
				}
				case 5:
				{
					auto& targetCellState = cell.second;
					targetCellState = inactive;
					break;
				}
				case 6:
				{
					auto& targetCellState = cell.second;
					targetCellState = active;
					break;
				}
				case 7:
				{
					auto& targetCellState = cell.second;
					targetCellState = inactive;
					break;
				}
				}

			}
		}
	}

}

void Elementary::draw_cells() const
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	auto map = m_cellMap;

	for (const auto& cell : m_cellMap)
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