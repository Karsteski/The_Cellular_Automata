#pragma once

#include "imgui.h"
#include <vector>
#include <memory>

class Grid
{
public:
	
	Grid();

	bool* enable_grid();
	
	ImVec2 get_min_canvas_position() const;
	ImVec2 get_max_canvas_position() const;
	ImVec2 get_canvas_size() const;

	float get_grid_steps() const;
	void set_grid_steps(float);

	void draw_grid();
	virtual void draw_cells(std::vector<ImVec2>) const;

	virtual ~Grid();

private:

	bool m_enable_grid;
	ImVec2 m_canvas_size;
	ImVec2 m_min_canvas_position;
	ImVec2 m_max_canvas_position;
	
	float m_grid_steps;

	ImU32 m_grid_square_colour_off;
	ImU32 m_grid_square_colour_on;

};
