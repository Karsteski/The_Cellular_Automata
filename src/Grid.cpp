#include "Grid.h"

Grid::Grid() : m_enable_grid(true), m_canvas_size(100.0f, 100.0f), m_min_canvas_position(0.0f, 0.0f), m_max_canvas_position(100.0f, 100.0f), m_grid_steps(10.0f),
m_grid_square_colour_off(255.0f, 255.0f, 255.0f, 255.0f), m_grid_square_colour_on(25.0f, 25.0f, 25.0f, 255.0f) {};

bool Grid::enable_grid(std::unique_ptr<bool> grid_flag)
{
	m_enable_grid = &grid_flag;
	return m_enable_grid;
}

ImVec2 Grid::get_min_canvas_position() const
{
	return m_min_canvas_position;
}

ImVec2 Grid::get_max_canvas_position() const
{
	return m_max_canvas_position;
}

ImVec2 Grid::get_canvas_size() const
{
	return m_canvas_size;
}

float Grid::get_grid_steps() const
{
	return m_grid_steps;
}

bool Grid::set_grid_steps(float grid_steps)
{
	if (grid_steps > 0)
	{
		m_grid_steps = grid_steps;
		return true;
	}

	else
	{
		return false;
	}
}

void Grid::draw_grid()
{
	m_min_canvas_position = ImGui::GetCursorScreenPos();
	m_canvas_size = ImGui::GetContentRegionAvail();

	// Resize canvas
	if (m_canvas_size.x < 50.0f) m_canvas_size.x = 50.0f;
	if (m_canvas_size.y < 50.0f) m_canvas_size.y = 50.0f;
	m_max_canvas_position = ImVec2(m_min_canvas_position.x + m_canvas_size.x, m_min_canvas_position.y + m_canvas_size.y);

	// Draw canvas border and background colour.
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddRectFilled(m_min_canvas_position, m_max_canvas_position, IM_COL32(0, 0, 0, 0));
	draw_list->AddRect(m_min_canvas_position, m_max_canvas_position, IM_COL32(255, 255, 255, 255));

	// Invisible button advances layout cursor + allows interaction with the grid.
	ImGui::InvisibleButton("Grid Invisible Button", m_canvas_size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_button_hovered = ImGui::IsItemHovered();
	const bool is_button_clicked = ImGui::IsItemActive();

	// Draw grid lines
	for (float x = m_grid_steps; x < m_canvas_size.x; x += m_grid_steps)
	{
		draw_list->AddLine(ImVec2(m_min_canvas_position.x + x, m_min_canvas_position.y), ImVec2(m_min_canvas_position.x + x, m_max_canvas_position.y), IM_COL32(200, 200, 200, 40));
	}

	for (float y = m_grid_steps; y < m_canvas_size.y; y += m_grid_steps)
	{
		draw_list->AddLine(ImVec2(m_min_canvas_position.x, m_min_canvas_position.y + y), ImVec2(m_max_canvas_position.x, m_min_canvas_position.y + y), IM_COL32(200, 200, 200, 40));
	}
}

// Incomplete.
// Implement the drawing of subsequent rectangles recursively. 
void Grid::draw_cells(std::vector<ImVec2>& cells_to_draw)
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->PushClipRect(m_min_canvas_position, m_max_canvas_position, true);

	draw_list->AddRectFilled(m_min_canvas_position, ImVec2(m_min_canvas_position.x + 10.0, m_min_canvas_position.y + 10.0),
		IM_COL32(100, 200, 100, 255));

	draw_list->PopClipRect();
}

Grid::~Grid()
{

}