#include "Elementary.h"

#include <iostream>
#include <algorithm>

// Default ruleset is rule 90.
Elementary::Elementary() : m_cellMap(), m_ruleset("01011010"), m_numberOfCellsPerGeneration(200), m_numberOfGenerations(200) { };

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
	m_cellMap.at(cell) = state;
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
	float startPoint = m_numberOfCellsPerGeneration / 2;
	SetSingleCellState(ImVec2(startPoint, 0), active);

	for (auto& cell : m_cellMap)
	{
		// Excludes cells(x, 0) as this is the initial generation that contains a single active cell.
		if (cell.first.y != 0)
		{
			std::bitset<3> previousCells("000");

			// Note that bitset reads from right to left, so 0 position is the furthest right bit.
			ImVec2 prev_cell_0 = ImVec2(cell.first.x + 1, cell.first.y - 1);
			ImVec2 prev_cell_1 = ImVec2(cell.first.x, cell.first.y - 1);
			ImVec2 prev_cell_2 = ImVec2(cell.first.x - 1, cell.first.y - 1);

			// Checks previous neighbour cells' states, which determines the current cell's state based on the rule bitset (m_ruleset).
			cellState defaultState = inactive;
			cellState& state_0 = (m_cellMap.find(prev_cell_0) != m_cellMap.end()) ? m_cellMap.at(prev_cell_0) : defaultState;
			cellState& state_1 = (m_cellMap.find(prev_cell_1) != m_cellMap.end()) ? m_cellMap.at(prev_cell_1) : defaultState;
			cellState& state_2 = (m_cellMap.find(prev_cell_2) != m_cellMap.end()) ? m_cellMap.at(prev_cell_2) : defaultState;

			if (state_0 == active) previousCells[0].flip();
			if (state_1 == active) previousCells[1].flip();
			if (state_2 == active) previousCells[2].flip();

			// Converts the bitset to a string for comparison using a switch statement.
			auto neighbourCellsState = previousCells.to_string();

			// This is the table that specifies a cell's state given its previous state, and the states of the cells to the left & right.
			std::string rulePosition_0 = "111";
			std::string rulePosition_1 = "110";
			std::string rulePosition_2 = "101";
			std::string rulePosition_3 = "100";
			std::string rulePosition_4 = "011";
			std::string rulePosition_5 = "010";
			std::string rulePosition_6 = "001";
			std::string rulePosition_7 = "000";

			// Can't switch on a string, so they're mapped to ints instead.
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

			// Problems here include C-style casts...
			switch (ruleMap.at(neighbourCellsState))
			{
			case 0:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(0));
				break;
			}
			case 1:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(1));
				break;
			}
			case 2:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(2));
				break;
			}
			case 3:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(3));
				break;
			}
			case 4:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(4));
				break;
			}
			case 5:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(5));
				break;
			}
			case 6:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(6));
				break;
			}
			case 7:
			{
				auto& targetCellState = cell.second;
				targetCellState = static_cast<cellState>(m_ruleset.test(7));
				break;
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

std::bitset<8>& Elementary::Ruleset()
{
	return m_ruleset;
}

Elementary::~Elementary()
{

}