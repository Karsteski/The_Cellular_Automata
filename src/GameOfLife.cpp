#include "GameOfLife.h"
#include <iostream>
#include <algorithm>
#include <ctime>

GameOfLife::GameOfLife() : m_cellMap(), m_gridDimensions(150.0f, 150.0f) { };

ImVec2 GameOfLife::GetGameDimensions()
{
	return m_gridDimensions;
}

void GameOfLife::SetGameDimensions(ImVec2 dimensions)
{
	if (dimensions.x > 0 and dimensions.y > 0)
	{
		m_gridDimensions.x = std::trunc(dimensions.x);
		m_gridDimensions.y = std::trunc(dimensions.y);
	}
}

std::map<ImVec2, CellState>& GameOfLife::GetCellMap()
{
	return m_cellMap;
}

void GameOfLife::GenerateEmptyCells()
{
	m_cellMap.clear();
	for (unsigned int x = 0; x < m_gridDimensions.x; ++x)
	{
		for (unsigned int y = 0; y < m_gridDimensions.y; ++y)
		{
			ImVec2 cell = ImVec2(x, y);
			CellState state = CellState::inactive;
			m_cellMap.insert(std::make_pair(cell, state));
		}
	}
}

void GameOfLife::GenerateRandomCells()
{
	m_cellMap.clear();
	// Time returns # of seconds since Jan 1st, 1970, making rand() seem truly random unless called w/in same second.
	std::srand(static_cast<int>(time(0)));

	for (unsigned int x = 0; x < m_gridDimensions.x; ++x)
	{
		for (unsigned int y = 0; y < m_gridDimensions.y; ++y)
		{
			ImVec2 cell = ImVec2(x, y);
			CellState state;
			
			if (rand() % 2)
				state = CellState::active;
			else
				state = CellState::inactive;
			m_cellMap.insert(std::make_pair(cell, state));
		}
	}
}

// Hard coded patterns for now.
// X = starting point, # = active cell, _ = inactive cell.
/*R-Pentonimo:
* X__##_
* __##__
* ___#__
*/

void GameOfLife::GeneratePattern(Pattern pattern)
{
	GenerateEmptyCells();
	ImVec2 startPoint = ImVec2(m_gridDimensions.x / 2, m_gridDimensions.y / 2);

	switch (pattern)
	{
	case Pattern::R_Pentomino:
	{
		ImVec2 cell_1 = ImVec2(startPoint.x + 3, startPoint.y);
		ImVec2 cell_2 = ImVec2(startPoint.x + 4, startPoint.y);
		ImVec2 cell_3 = ImVec2(startPoint.x + 2, startPoint.y + 1);
		ImVec2 cell_4 = ImVec2(startPoint.x + 3, startPoint.y + 1);
		ImVec2 cell_5 = ImVec2(startPoint.x + 3, startPoint.y + 2);
		std::vector<ImVec2> testVector = { cell_1, cell_2, cell_3, cell_4, cell_5 };
		for (auto& [cell, state] : m_cellMap)
		{
			for (auto& testCell : testVector)
			{
				// Sigh need to overload "==" for ImVec2...
				if (cell.x == testCell.x and cell.y == testCell.y)
					state = CellState::active;
			}
		}

		break;
	}
	case Pattern::Glider_Gun:
	{

		// Currently unimplemented as it'd take too long to hardcode this...
		break;
	}
	case Pattern::Infinite_Growth:
	{
		startPoint = ImVec2(m_gridDimensions.x / 3, m_gridDimensions.y / 2);

		ImVec2 cell_1 = ImVec2(startPoint.x + 1, startPoint.y);
		ImVec2 cell_2 = ImVec2(startPoint.x + 2, startPoint.y);
		ImVec2 cell_3 = ImVec2(startPoint.x + 3, startPoint.y);
		ImVec2 cell_4 = ImVec2(startPoint.x + 4, startPoint.y);
		ImVec2 cell_5 = ImVec2(startPoint.x + 5, startPoint.y);
		ImVec2 cell_6 = ImVec2(startPoint.x + 6, startPoint.y);
		ImVec2 cell_7 = ImVec2(startPoint.x + 7, startPoint.y);
		ImVec2 cell_8 = ImVec2(startPoint.x + 8, startPoint.y);

		ImVec2 cell_9 = ImVec2(startPoint.x + 10, startPoint.y);
		ImVec2 cell_10 = ImVec2(startPoint.x + 11, startPoint.y);
		ImVec2 cell_11 = ImVec2(startPoint.x + 12, startPoint.y);
		ImVec2 cell_12 = ImVec2(startPoint.x + 13, startPoint.y);
		ImVec2 cell_13 = ImVec2(startPoint.x + 14, startPoint.y);

		ImVec2 cell_14 = ImVec2(startPoint.x + 18, startPoint.y);
		ImVec2 cell_15 = ImVec2(startPoint.x + 19, startPoint.y);
		ImVec2 cell_16 = ImVec2(startPoint.x + 20, startPoint.y);

		ImVec2 cell_17 = ImVec2(startPoint.x + 27, startPoint.y);
		ImVec2 cell_18 = ImVec2(startPoint.x + 28, startPoint.y);
		ImVec2 cell_19 = ImVec2(startPoint.x + 29, startPoint.y);
		ImVec2 cell_20 = ImVec2(startPoint.x + 30, startPoint.y);
		ImVec2 cell_21 = ImVec2(startPoint.x + 31, startPoint.y);
		ImVec2 cell_22 = ImVec2(startPoint.x + 32, startPoint.y);
		ImVec2 cell_23 = ImVec2(startPoint.x + 33, startPoint.y);

		ImVec2 cell_24 = ImVec2(startPoint.x + 35, startPoint.y);
		ImVec2 cell_25 = ImVec2(startPoint.x + 36, startPoint.y);
		ImVec2 cell_26 = ImVec2(startPoint.x + 37, startPoint.y);
		ImVec2 cell_27 = ImVec2(startPoint.x + 38, startPoint.y);
		ImVec2 cell_28 = ImVec2(startPoint.x + 39, startPoint.y);

		std::vector<ImVec2> testVector = 
		{ 
		cell_1, cell_2, cell_3, cell_4, cell_5, cell_6, cell_7, cell_8, cell_9, cell_10,
		cell_11, cell_12, cell_13, cell_14, cell_15, cell_16, cell_17, cell_18, cell_19, 
		cell_20, cell_21, cell_22, cell_23, cell_24, cell_25, cell_26, cell_27, cell_28
		};

		for (auto& [cell, state] : m_cellMap)
		{
			for (auto& testCell : testVector)
			{
				// Sigh need to overload "==" for ImVec2...
				if (cell.x == testCell.x and cell.y == testCell.y)
					state = CellState::active;
			}
		}

		break;
	}
	}
}

bool GameOfLife::SetSingleCellState(ImVec2 cell, CellState state)
{
	if (m_cellMap.find(cell) != m_cellMap.end())
	{
		m_cellMap[cell] = state;
		return true;
	}
	else
	{
		return false;
	}
}

CellState GameOfLife::GetCellState(ImVec2 cell)
{
	if (m_cellMap.find(cell) != m_cellMap.end())
	{
		auto state = m_cellMap.at(cell);
		return state;
	}
	else
	{
		return CellState::inactive;
	}
}

void GameOfLife::SetAllCellStates()
{
	// Buffer as I can't read and write to the cellMap at the same time, as the cells written to affect the next cells.
	std::map<ImVec2, CellState> cellMapBuffer;

	// Can't pass by reference for the above reason. Expensive copy!
	for (auto [cell, state] : m_cellMap)
	{
		//Cell positions relative to the current cell (C). 
		/*
		0 1 2
		3 C 4
		5 6 7
		*/
		ImVec2 prev_cell_0 = ImVec2(cell.x - 1, cell.y - 1);
		ImVec2 prev_cell_1 = ImVec2(cell.x + 0, cell.y - 1);
		ImVec2 prev_cell_2 = ImVec2(cell.x + 1, cell.y - 1);
		ImVec2 prev_cell_3 = ImVec2(cell.x - 1, cell.y + 0);
		ImVec2 prev_cell_4 = ImVec2(cell.x + 1, cell.y + 0);
		ImVec2 prev_cell_5 = ImVec2(cell.x - 1, cell.y + 1);
		ImVec2 prev_cell_6 = ImVec2(cell.x + 0, cell.y + 1);
		ImVec2 prev_cell_7 = ImVec2(cell.x + 1, cell.y + 1);

		// Checks previous neighbour cells' states
		CellState defaultState = CellState::inactive;
		CellState& state_0 = (m_cellMap.find(prev_cell_0) != m_cellMap.end()) ? m_cellMap.at(prev_cell_0) : defaultState;
		CellState& state_1 = (m_cellMap.find(prev_cell_1) != m_cellMap.end()) ? m_cellMap.at(prev_cell_1) : defaultState;
		CellState& state_2 = (m_cellMap.find(prev_cell_2) != m_cellMap.end()) ? m_cellMap.at(prev_cell_2) : defaultState;
		CellState& state_3 = (m_cellMap.find(prev_cell_3) != m_cellMap.end()) ? m_cellMap.at(prev_cell_3) : defaultState;
		CellState& state_4 = (m_cellMap.find(prev_cell_4) != m_cellMap.end()) ? m_cellMap.at(prev_cell_4) : defaultState;
		CellState& state_5 = (m_cellMap.find(prev_cell_5) != m_cellMap.end()) ? m_cellMap.at(prev_cell_5) : defaultState;
		CellState& state_6 = (m_cellMap.find(prev_cell_6) != m_cellMap.end()) ? m_cellMap.at(prev_cell_6) : defaultState;
		CellState& state_7 = (m_cellMap.find(prev_cell_7) != m_cellMap.end()) ? m_cellMap.at(prev_cell_7) : defaultState;

		unsigned int numberOfActiveNeighbours = 0;
		if (state_0 == CellState::active) ++numberOfActiveNeighbours;
		if (state_1 == CellState::active) ++numberOfActiveNeighbours;
		if (state_2 == CellState::active) ++numberOfActiveNeighbours;
		if (state_3 == CellState::active) ++numberOfActiveNeighbours;
		if (state_4 == CellState::active) ++numberOfActiveNeighbours;
		if (state_5 == CellState::active) ++numberOfActiveNeighbours;
		if (state_6 == CellState::active) ++numberOfActiveNeighbours;
		if (state_7 == CellState::active) ++numberOfActiveNeighbours;

		switch (state)
		{
		case (CellState::active):
		{
			if (numberOfActiveNeighbours < 2) // Cell dies by underpopulation.
				state = CellState::inactive;
			else if (numberOfActiveNeighbours == 2 or numberOfActiveNeighbours == 3) // Cell is happy and lives on :)
				state = CellState::active;
			else if (numberOfActiveNeighbours > 3) // Cell dies by overpopulation.
				state = CellState::inactive;
			break;
		}
		case (CellState::inactive):
		{
			if (numberOfActiveNeighbours == 3) // Cells reproduce to create a live cell. 
				state = CellState::active;
			break;
		}
		}

		cellMapBuffer.insert(std::make_pair(cell, state));
	}

	m_cellMap = cellMapBuffer;
}

void GameOfLife::DrawCells() const
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	for (const auto& [cell, state] : m_cellMap)
	{
		if (state == CellState::active)
		{
			const ImVec2 origin = ImVec2(m_min_canvas_position.x + m_grid_scrolling.x, m_min_canvas_position.y + m_grid_scrolling.y);
			const ImVec2 cell_pos_i = ImVec2(origin.x + (cell.x * m_grid_steps), origin.y + (cell.y * m_grid_steps));
			const ImVec2 cell_pos_f = ImVec2(cell_pos_i.x + m_grid_steps, cell_pos_i.y + m_grid_steps);

			draw_list->PushClipRect(m_min_canvas_position, m_max_canvas_position, true);
			draw_list->AddRectFilled(cell_pos_i, cell_pos_f, m_cell_colour_off);
			draw_list->PopClipRect();
		}
		else
		{
			const ImVec2 origin = ImVec2(m_min_canvas_position.x + m_grid_scrolling.x, m_min_canvas_position.y + m_grid_scrolling.y);
			const ImVec2 cell_pos_i = ImVec2(origin.x + (cell.x * m_grid_steps), origin.y + (cell.y * m_grid_steps));
			const ImVec2 cell_pos_f = ImVec2(cell_pos_i.x + m_grid_steps, cell_pos_i.y + m_grid_steps);

			draw_list->PushClipRect(m_min_canvas_position, m_max_canvas_position, true);
			draw_list->AddRectFilled(cell_pos_i, cell_pos_f, m_cell_colour_on);
			draw_list->PopClipRect();
		}
	}
}

void GameOfLife::GenerateGameOfLife()
{
	SetAllCellStates();
}

GameOfLife::~GameOfLife() = default;