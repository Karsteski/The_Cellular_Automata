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

	float GetGridSteps() const;
	void SetGridSteps(unsigned int);

	void SetMainCellColour(ImColor);

	void DrawGrid();
	virtual void DrawCells();

	virtual ~Grid();

private:

	
	bool m_enable_grid;
	ImVec2 m_canvas_size;
	ImVec2 m_min_canvas_position;
	ImVec2 m_max_canvas_position;
	
	ImVec2 m_grid_scrolling;
	unsigned int m_grid_steps;

	ImColor m_cell_colour_main;

	friend class Elementary;
	friend class GameOfLife;
};
