#include "ConwayEngine.h"

ConwayEngine::ConwayEngine() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::mt19937::result_type> distrib(-10, 10);

	for (int i = 0; i < 100; i++) {
		int x = distrib(gen);
		int y = distrib(gen);


		auto pair = { x,y };
		initialBoard[{x, y}] = true;
		board[{x, y}] = true;
	}
}

ConwayEngine::ConwayEngine(unsigned initialAliveCells, unsigned spreadFactor) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 100 * spreadFactor);

	for (int i = 0; i < initialAliveCells; i++) {
		int x = distrib(gen);
		int y = distrib(gen);

		auto pair = { x,y };
		initialBoard[{x,y}] = 1;
		board[{x, y}] = 1;
	}

}

int ConwayEngine::countNeighbors(int x, int y) {
	int neighbors = 0;

	auto it = board.begin();
	for (const auto& dirArr : DIRECTIONS) {
		int newX = x + dirArr[0];
		int newY = y + dirArr[1];

		it = board.find({ newX, newY });
		if (it != board.end()) {
			neighbors++;
		}
	}

	return neighbors;
}

void ConwayEngine::updateNeighborMatrix() {
	for (auto it = board.begin(); it != board.end(); it++) {
		int x = it->first.first;
		int y = it->first.second;
		neighborMatrix[it->first] = countNeighbors(x, y);
		for (const auto& dirArr : DIRECTIONS) {
			int newX = x + dirArr[0];
			int newY = y + dirArr[1];
			neighborMatrix[{newX, newY}] = countNeighbors(newX, newY);
		}
	}
}

void ConwayEngine::runConwayRules() {
	auto it = board.begin();
	while (it != board.end()) {
		// it->first is the pair used as key in dict

		int numNeighbors = neighborMatrix[{it->first}];

		if (numNeighbors < 2 || numNeighbors > 3) {
			it = board.erase(it);
		}
		else {
			it++;
		}
	}

	// Now iterate through neighbors and revive cells that
	// had exactly 3 neighbors
	it = neighborMatrix.begin();
	while (it != neighborMatrix.end()) {

		auto boardIter = board.find(it->first);

		if (boardIter == board.end()) {
			// Means this {x, y} not found in board
			int numNeighbors = it->second;
			if (numNeighbors == 3) {
				board.insert({ {it->first}, true });
			}
		}

		it++;
	}
}

void ConwayEngine::nextGeneration() {
	updateNeighborMatrix();
	runConwayRules();
}

const ConwayEngine::SparseMatrix& ConwayEngine::currentGeneration() {
	return board;
}