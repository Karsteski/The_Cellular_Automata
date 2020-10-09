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
	
	const std::map<ImVec2, CellState>& GetCellMap() const;
	const int GetNumberOfGenerations() const;
	const CellState GetCellState(ImVec2) const;
	
	std::bitset<8>& SetRuleset();
	void SetNumberOfGenerations(int);
	bool SetSingleCellState(ImVec2, CellState);
	
	void GenerateCells(CellState);
	
	void SetAllCellStates();

	virtual void DrawCells() override;

	void GenerateElementaryAutomata();

	virtual ~Elementary();

private:

	std::map<ImVec2, CellState> m_cellMap;
	std::bitset<8> m_ruleset;
	
	unsigned int m_numberOfCellsPerGeneration;
	unsigned int m_numberOfGenerations;

};

