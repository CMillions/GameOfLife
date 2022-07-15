#pragma once

#include <map>
#include <random>

class ConwayEngine
{
public:
	typedef std::map<std::pair<int, int>, int> SparseMatrix;
private:

	const int DIRECTIONS[8][2] = {
		{-1, -1},
		{ 0, -1},
		{ 1, -1},
		{-1,  0},
		{ 1,  0},
		{-1,  1},
		{ 0,  1},
		{ 1,  1}
	};

	SparseMatrix initialBoard;

	SparseMatrix board;
	SparseMatrix neighborMatrix;

	int countNeighbors(int x, int y);

	void updateNeighborMatrix();
	void runConwayRules();


public:
	ConwayEngine();
	ConwayEngine(unsigned initialAliveCells, unsigned spreadFactor);

	void nextGeneration();
	const SparseMatrix& currentGeneration();

};

