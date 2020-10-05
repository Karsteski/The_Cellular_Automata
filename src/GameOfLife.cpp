#include "GameOfLife.h"
#include <iostream>
#include <algorithm>

GameOfLife::GameOfLife() : m_cellMap(), m_gridDimensions(100.0f, 100.0f) { };

std::map<ImVec2, CellState>& GameOfLife::GetCellMap()
{
	return m_cellMap;
}

void GameOfLife::GenerateCells()
{
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
	for (auto& [cell, state] : m_cellMap)
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
			else
				state = CellState::inactive;
			break;
		}
		default:
			state = CellState::inactive;
		}
	}
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