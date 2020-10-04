#include "Grid.h"
#include <utility>
#include <cmath>

Grid::Grid() : m_enable_grid(true), m_canvas_size(100.0f, 100.0f), m_min_canvas_position(0.0f, 0.0f), m_max_canvas_position(100.0f, 100.0f), m_grid_scrolling(ImVec2(0.0f, 0.0f)),
m_grid_steps(10.0f),
m_cell_colour_main(IM_COL32(100.0f, 100.0f, 100.0f, 255.0f)),
m_cell_colour_off(IM_COL32(255.0f, 255.0f, 255.0f, 255.0f)),
m_cell_colour_on(IM_COL32(25.0f, 25.0f, 25.0f, 255.0f)) {};

// Probably want to convert this to a smart pointer...
void Grid::EnableGrid(bool input)
{
	m_enable_grid = input;
}

ImVec2 Grid::GetMinimumCanvasPosition() const
{
	return m_min_canvas_position;
}

ImVec2 Grid::GetMaximumCanvasPosition() const
{
	return m_max_canvas_position;
}

ImVec2 Grid::GetCanvasSize() const
{
	return m_canvas_size;
}

ImColor Grid::GetCellMainCellColour()
{
	return m_cell_colour_main;
}

float Grid::GetGridSteps() const
{
	return m_grid_steps;
}

// Only use positive integers or the negative sign will be dropped, as well as any decimals.
void Grid::SetGridSteps(unsigned int gridSteps)
{
	m_grid_steps = gridSteps;
}

void Grid::SetMainCellColour(ImColor colour)
{
	m_cell_colour_main = colour;
}

void Grid::DrawGrid()
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
	const bool is_mouse_clicked = ImGui::IsItemActive();

	// Pan around the grid.
	// Can make threshold for panning dynamic depending on whether or not mouse is hovering over a widget, for example.
	const float mouse_threshold_for_panning = 0.0f;
	if (is_mouse_clicked && ImGui::IsMouseDragging(ImGuiMouseButton_Left, mouse_threshold_for_panning))
	{
		m_grid_scrolling.x += io.MouseDelta.x;
		m_grid_scrolling.y += io.MouseDelta.y;
	}

	if (m_enable_grid)
	{
		// Draw grid lines
		draw_list->PushClipRect(m_min_canvas_position, m_max_canvas_position, true);
		for (float x = std::fmod(m_grid_scrolling.x, m_grid_steps); x < m_canvas_size.x; x += m_grid_steps)
		{
			draw_list->AddLine(ImVec2(m_min_canvas_position.x + x, m_min_canvas_position.y), ImVec2(m_min_canvas_position.x + x, m_max_canvas_position.y), IM_COL32(200, 200, 200, 40));
		}

		for (float y = std::fmod(m_grid_scrolling.y, m_grid_steps); y < m_canvas_size.y; y += m_grid_steps)
		{
			draw_list->AddLine(ImVec2(m_min_canvas_position.x, m_min_canvas_position.y + y), ImVec2(m_max_canvas_position.x, m_min_canvas_position.y + y), IM_COL32(200, 200, 200, 40));
		}
		draw_list->PopClipRect();
	}
}

// Only use positive integers or the negative sign will be dropped, as well as any decimals.
void Grid::DrawCells(std::vector<ImVec2>& cells_to_draw) const
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	for (auto cell : cells_to_draw)
	{
		// To remove negative numbers, which don't show up on the grid.
		if (cell.x < 0 or cell.y < 0)
		{
			cell.x = std::abs(cell.x);
			cell.y = std::abs(cell.y);
		}

		// To remove decimals.
		{
			cell.x = std::trunc(cell.x);
			cell.y = std::trunc(cell.y);
		}

		// Note that std::fmod is simply moving the grid by a factor of the grid_steps.
		const ImVec2 origin = ImVec2(m_min_canvas_position.x + m_grid_scrolling.x, m_min_canvas_position.y + m_grid_scrolling.y);
		ImVec2 cell_pos_i = ImVec2(origin.x + (cell.x * m_grid_steps), origin.y + (cell.y * m_grid_steps));
		ImVec2 cell_pos_f = ImVec2(cell_pos_i.x + m_grid_steps, cell_pos_i.y + m_grid_steps);

		draw_list->PushClipRect(m_min_canvas_position, m_max_canvas_position, true);
		draw_list->AddRectFilled(cell_pos_i, cell_pos_f, m_cell_colour_main);
		draw_list->PopClipRect();
	}
}

Grid::~Grid() = default;