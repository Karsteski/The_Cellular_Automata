#pragma once
#include "Elementary.h"

#include <map>
#include <bitset>
#include <utility>

#include "Grid.h"
#include "imgui.h"



class GameOfLife : public Grid
{

public:

	GameOfLife();

	std::map<ImVec2, CellState>& GetCellMap();

	CellState GetCellState(ImVec2);
	void GenerateCells();

	bool SetSingleCellState(ImVec2, CellState);

	void SetAllCellStates();

	virtual void DrawCells() const;

	void GenerateGameOfLife();

	virtual ~GameOfLife();

private:

	std::map<ImVec2, CellState> m_cellMap;
	ImVec2 m_gridDimensions;

};
