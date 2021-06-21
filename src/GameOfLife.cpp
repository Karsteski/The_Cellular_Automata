#include "GameOfLife.h"

#include <chrono>
#include <cmath>
#include <future>
#include <thread>

GameOfLife::GameOfLife()
    : m_cellMap()
    , m_gridDimensions(150.0f, 150.0f) {}

ImVec2 GameOfLife::GetGameDimensions()
{
    return m_gridDimensions;
}

void GameOfLife::SetGameDimensions(ImVec2 dimensions)
{
    if (dimensions.x > 0 && dimensions.y > 0) {
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
    for (int x = 0; x < m_gridDimensions.x; ++x) {
        for (int y = 0; y < m_gridDimensions.y; ++y) {
            ImVec2 cell = ImVec2(x, y);
            CellState state = CellState::inactive;
            m_cellMap.insert(std::make_pair(cell, state));
        }
    }
}

void GameOfLife::GenerateRandomCells()
{
    m_cellMap.clear();
    // Time returns # of seconds since Jan 1st, 1970, making rand() seem truly random unless called within the same second.
    std::srand(static_cast<int>(time(0)));

    for (int x = 0; x < m_gridDimensions.x; ++x) {
        for (int y = 0; y < m_gridDimensions.y; ++y) {
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

void GameOfLife::GeneratePattern(Pattern pattern)
{
    GenerateEmptyCells();

    // 'X' = starting point (0, 0), '#' = active cell, '.' = inactive cell.
    const auto StringPatternToCells = [&](const std::string& inputString) {
        int gridRow = 0;
        int gridColumn = 0;
        std::vector<ImVec2> cellsToWrite;
        ImVec2 startPoint = ImVec2(m_gridDimensions.x / 3, m_gridDimensions.y / 2);
        for (const auto& stringCell : inputString) {
            ++gridRow;

            if (stringCell == '#') {
                cellsToWrite.push_back(ImVec2(startPoint.x + gridRow, startPoint.y + gridColumn));
            } else if (stringCell == '\n') {
                gridRow = 0;
                ++gridColumn;
            }
        }

        return cellsToWrite;
    };

    auto cellTester = [&](const std::vector<ImVec2>& inputVector) {
        for (auto& [cell, state] : m_cellMap) {
            for (auto& testCell : inputVector) {
                if (m_cellMap.find(testCell) != m_cellMap.end()) {
                    // Sigh need to overload "==" for ImVec2... or switch to std::pair.
                    if (cell.x == testCell.x && cell.y == testCell.y)
                        state = CellState::active;
                }
            }
        }
    };

    switch (pattern) {
    case Pattern::R_Pentomino: {
        const std::string R_Pentomino = "X..##.\n"
                                        "..##..\n"
                                        "...#..";

        const auto testVector = StringPatternToCells(R_Pentomino);
        cellTester(testVector);

        break;
    }
    case Pattern::Glider_Gun: {
        const std::string Glider_Gun = "X.......................#...........\n"
                                       "......................#.#...........\n"
                                       "............##......##............##\n"
                                       "...........#...#....##............##\n"
                                       "##........#.....#...##..............\n"
                                       "##........#...#.##....#.#...........\n"
                                       "..........#.....#.......#...........\n"
                                       "...........#...#....................\n"
                                       "............##......................";

        const auto testVector = StringPatternToCells(Glider_Gun);
        cellTester(testVector);

        break;
    }
    case Pattern::Infinite_Growth: {
        const std::string Infinite_Growth = "X########.#####...###......#######.#####";

        const auto testVector = StringPatternToCells(Infinite_Growth);
        cellTester(testVector);

        break;
    }
    }
}

bool GameOfLife::SetSingleCellState(ImVec2 cell, CellState state)
{
    if (m_cellMap.find(cell) != m_cellMap.end()) {
        m_cellMap[cell] = state;
        return true;
    } else {
        return false;
    }
}

CellState GameOfLife::GetCellState(ImVec2 cell)
{
    if (m_cellMap.find(cell) != m_cellMap.end()) {
        const auto state = m_cellMap.at(cell);
        return state;
    } else {
        return CellState::inactive;
    }
}
void GameOfLife::SetAllCellStates()
{
    // Buffer as I can't read and write to the cellMap at the same time, as the cells written to affect the next cells.
    std::map<ImVec2, CellState> cellMapBuffer;

    // Can't pass by reference for the above reason. Expensive copy!
    for (auto [cell, state] : m_cellMap) {
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

        int numberOfActiveNeighbours = 0;
        if (state_0 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_1 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_2 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_3 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_4 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_5 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_6 == CellState::active)
            ++numberOfActiveNeighbours;
        if (state_7 == CellState::active)
            ++numberOfActiveNeighbours;

        switch (state) {
        case (CellState::active): {
            if (numberOfActiveNeighbours < 2) // Cell dies by underpopulation.
                state = CellState::inactive;
            else if (numberOfActiveNeighbours == 2 || numberOfActiveNeighbours == 3) // Cell is happy and lives on :)
                state = CellState::active;
            else if (numberOfActiveNeighbours > 3) // Cell dies by overpopulation.
                state = CellState::inactive;
            break;
        }
        case (CellState::inactive): {
            if (numberOfActiveNeighbours == 3) // Cells reproduce to create a live cell.
                state = CellState::active;
            break;
        }
        }

        cellMapBuffer.insert(std::make_pair(cell, state));
    }

    m_cellMap = cellMapBuffer;
}

void GameOfLife::DrawCells()
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const ImVec2 origin = ImVec2(m_min_canvas_position.x + m_grid_scrolling.x, m_min_canvas_position.y + m_grid_scrolling.y);

    draw_list->PushClipRect(m_min_canvas_position, m_max_canvas_position, true);
    draw_list->AddRect(origin, ImVec2(origin.x + (m_gridDimensions.x * m_grid_steps), origin.y + (m_gridDimensions.y * m_grid_steps)), IM_COL32(200, 200, 200, 255));

    for (const auto& [cell, state] : m_cellMap) {
        if (state == CellState::active) {
            const ImVec2 cell_pos_i = ImVec2(origin.x + (cell.x * m_grid_steps), origin.y + (cell.y * m_grid_steps));
            const ImVec2 cell_pos_f = ImVec2(cell_pos_i.x + m_grid_steps, cell_pos_i.y + m_grid_steps);

            draw_list->AddRectFilled(cell_pos_i, cell_pos_f, m_cell_colour_main);
        }
    }

    draw_list->PopClipRect();
}

void GameOfLife::GenerateGameOfLife()
{
    SetAllCellStates();
    DrawGrid();
    DrawCells();
}