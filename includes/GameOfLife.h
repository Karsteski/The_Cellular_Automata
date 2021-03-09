#pragma once
#include "Elementary.h"

#include <map>
#include <bitset>
#include <utility>
#include <string>

#include "Grid.h"
#include "imgui.h"

enum class Pattern : int { R_Pentomino = 0, Glider_Gun = 1, Infinite_Growth = 2 };

class GameOfLife : public Grid
{

public:

	GameOfLife();

	ImVec2 GetGameDimensions();
	void SetGameDimensions(ImVec2);

	std::map<ImVec2, CellState>& GetCellMap();

	CellState GetCellState(ImVec2);
	
	void GenerateEmptyCells();
	void GenerateRandomCells();
	void GeneratePattern(Pattern);

	bool SetSingleCellState(ImVec2, CellState);

	void SetAllCellStates();

	void DrawCells() override;

	void GenerateGameOfLife();

private:

	std::map<ImVec2, CellState> m_cellMap;
	ImVec2 m_gridDimensions;
};
