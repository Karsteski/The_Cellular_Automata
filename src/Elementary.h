#pragma once


#include <map>
#include <bitset>
#include <utility>

#include "Grid.h"
#include "imgui.h"

#include "STL_Extensions.h"

enum class CellState : bool { inactive = false, active = true };

class Elementary : public Grid
{

public:

	Elementary();
	
	std::map<ImVec2, CellState>& GetCellMap();
	unsigned int GetNumberOfGenerations();

	CellState GetCellState(ImVec2);
	void GenerateCells(CellState);
	
	bool SetSingleCellState(ImVec2, CellState);
	void SetNumberOfGenerations(unsigned int);
	
	void SetAllCellStates();

	virtual void DrawCells() const;

	std::bitset<8>& Ruleset();

	void GenerateElementaryAutomata();

	virtual ~Elementary();

private:

	std::map<ImVec2, CellState> m_cellMap;
	std::bitset<8> m_ruleset;
	
	unsigned int m_numberOfCellsPerGeneration;
	unsigned int m_numberOfGenerations;

};
