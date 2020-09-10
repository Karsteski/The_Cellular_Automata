#include "Elementary.h"

#include <iostream>

Elementary::Elementary() : m_cellMap(), m_ruleset("00000000"), m_numberOfCellsPerGeneration(32), m_numberOfGenerations(100) { };

std::map<ImVec2, Elementary::cellState>& Elementary::GetCellMap()
{
	return m_cellMap;
}

void Elementary::GenerateCells()
{
	for (unsigned int generation = 0; generation < m_numberOfGenerations; ++generation)
	{
		for (unsigned int position = 0; position < m_numberOfCellsPerGeneration; ++position)
		{
			ImVec2 cell = ImVec2(position, generation);
			cellState defaultCellState = inactive;

			m_cellMap.insert(std::make_pair(cell, inactive));

			std::cout << "(" << position << ", " << generation << ") - " << defaultCellState << std::endl;

		}
	}	
}

void Elementary::GetNeighbourCells()
{

}

bool Elementary::GetCellState()
{
	return 0;
}

void Elementary::SetAllCellStates(std::map<ImVec2, cellState>& cellMap)
{

}

void Elementary::draw_cells() const
{

}

unsigned int Elementary::GetNumberOfGenerations()
{
	return 0;
}

void Elementary::SetNumberOfGenerations(unsigned int number)
{

}

void Elementary::SetRandomRuleset()
{

}

Elementary::~Elementary()
{

}