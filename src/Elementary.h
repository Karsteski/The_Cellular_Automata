#pragma once

#include "Grid.h"
#include "imgui.h"
#include <map>
#include <bitset>
#include <utility>

// So that ImVec2 can be used with std::map
namespace std
{
	template<> struct less<ImVec2>
	{
		bool operator() (const ImVec2& lhs, const ImVec2& rhs) const
		{
			if (lhs.x == rhs.x)
				return (lhs.y < rhs.y);
			else
				return (lhs.x < rhs.x);
		}
	};
}

enum cellState : bool { inactive = false, active = true };

class Elementary : public Grid
{

public:

	

	Elementary();
	
	std::map<ImVec2, cellState>& GetCellMap();
	unsigned int GetNumberOfGenerations();

	cellState GetCellState(ImVec2);
	void GenerateCells(cellState);
	
	void SetSingleCellState(ImVec2, cellState);
	void SetNumberOfGenerations(unsigned int);
	
	void SetAllCellStates();

	virtual void draw_cells() const;

	
	
	void SetRandomRuleset();

	virtual ~Elementary();

private:

	
	std::map<ImVec2, cellState> m_cellMap;
	std::bitset<8> m_ruleset;
	
	unsigned int m_numberOfCellsPerGeneration;
	unsigned int m_numberOfGenerations;

};

