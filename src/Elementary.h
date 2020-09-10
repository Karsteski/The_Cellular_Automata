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

class Elementary : public Grid
{

public:

	enum cellState : bool { inactive = false, active = true };

	Elementary();
	
	std::map<ImVec2, cellState>& GetCellMap();
	unsigned int GetNumberOfGenerations();

	void GetNeighbourCells();
	bool GetCellState();
	void GenerateCells();
	
	void SetNumberOfGenerations(unsigned int);
	
	void SetAllCellStates(std::map<ImVec2, cellState>&);

	virtual void draw_cells() const;

	
	
	void SetRandomRuleset();

	virtual ~Elementary();

private:

	
	std::map<ImVec2, cellState> m_cellMap;
	std::bitset<8> m_ruleset;
	
	unsigned int m_numberOfCellsPerGeneration;
	unsigned int m_numberOfGenerations;

};

