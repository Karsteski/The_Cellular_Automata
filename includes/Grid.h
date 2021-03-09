#pragma once

#include "imgui.h"
#include <vector>

class Grid
{
public:

	std::vector <ImVec2> m_cells_to_draw;
	
	Grid();

	void EnableGrid(bool);
	
	ImVec2 GetMinimumCanvasPosition() const;
	ImVec2 GetMaximumCanvasPosition() const;
	ImVec2 GetCanvasSize() const;

	ImColor GetCellMainCellColour();

	int GetGridSteps() const;
	void SetGridSteps(int);

	void SetMainCellColour(ImColor);

	void DrawGrid();
	virtual void DrawCells();

	// Following the Rule of 5. 
	// No use for the special member functions, so they are simply deleted.
	Grid(const Grid&) = delete;
	Grid(Grid&&) = delete;
	Grid& operator=(const Grid&) = delete;
	Grid& operator=(Grid&&) = delete;

	virtual ~Grid() = default;

private:

	bool m_enable_grid;
	ImVec2 m_canvas_size;
	ImVec2 m_min_canvas_position;
	ImVec2 m_max_canvas_position;
	
	ImVec2 m_grid_scrolling;
	int m_grid_steps;

	ImColor m_cell_colour_main;

	friend class Elementary;
	friend class GameOfLife;
};
