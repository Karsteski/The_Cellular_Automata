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

	ImColor get_cell_colour_main();

	float get_grid_steps() const;
	void set_grid_steps(float);

	void set_cell_colour_main(ImColor);

	void draw_grid();
	virtual void draw_cells(std::vector<ImVec2>) const;

	virtual ~Grid();

private:

	bool m_enable_grid;
	ImVec2 m_canvas_size;
	ImVec2 m_min_canvas_position;
	ImVec2 m_max_canvas_position;
	
	float m_grid_steps;

	ImColor m_cell_colour_main;
	ImColor m_cell_colour_off;
	ImColor m_cell_colour_on;

	friend class Elementary;

};
