#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <random>

enum CellStatus {
	ALIVE,
	DEAD
};

// Need to check 8 directions when
// counting neighbors
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

typedef std::map<std::pair<int, int>, int> SparseMatrix;

int countNeighbors(int x, int y, const SparseMatrix& board) {
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


void updateNeighborMatrix(const SparseMatrix& board, SparseMatrix& neighborMatrix) {
	
	for (auto it = board.begin(); it != board.end(); it++) {
		int x = it->first.first;
		int y = it->first.second;
		neighborMatrix[it->first] = countNeighbors(x, y, board);
		for (const auto& dirArr : DIRECTIONS) {
			int newX = x + dirArr[0];
			int newY = y + dirArr[1];
			neighborMatrix[{newX, newY}] = countNeighbors(newX, newY, board);
		}
	}
}

void conway(SparseMatrix& board, SparseMatrix& neighorMatrix) {
	auto it = board.begin();
	while (it != board.end()) {

		// it->first is the pair used as key in dict

		int numNeighbors = neighorMatrix[{it->first}];

		if (numNeighbors < 2 || numNeighbors > 3) {
			it = board.erase(it);
		}
		else {
			it++;
		}
	}
	
	// Now iterate through neighbors and revive cells that
	// had exactly 3 neighbors
	it = neighorMatrix.begin();
	while (it != neighorMatrix.end()) {

		auto boardIter = board.find(it->first);
		if (boardIter == board.end()) {
			// Means this {x, y} not found in board
			int numNeighbors = it->second;
			if (numNeighbors == 3) {
				board.insert({ {it->first}, CellStatus::ALIVE });
			}
		}

		it++;
	}
}

#include "App.h"

int main() {

	const unsigned int WIDTH = 800u;
	const unsigned int HEIGHT = 600u;

	App app(WIDTH, HEIGHT, "Game of Life");
	app.setFramerateLimit(144);
	app.start();
	

	sf::RenderWindow window({WIDTH, HEIGHT}, "SFML");
	window.setFramerateLimit(144);
	sf::View camera(sf::Vector2f(WIDTH / 2.f, HEIGHT / 2.f), sf::Vector2f(WIDTH, HEIGHT));

	window.setView(camera);

	SparseMatrix board;
	SparseMatrix neighborsMatrix;

	sf::Clock timer;
	float elapsedTime = 0.f;

	const float UPDATES_PER_SECOND = 4;
	const float SECONDS_PER_UPDATE = 1.0f / UPDATES_PER_SECOND;

	const int SQUARE_SIZE = 16;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randomGen(0, 100);

	for (int i = 0; i < 10000; i++) {
		int x = randomGen(rng);
		int y = randomGen(rng);

		board[{x, y}] = CellStatus::ALIVE;
	}


	/*
	 * Use vertexArray to hold vertices for alive squares.
	 * 1      2
	 * ========  Vertices are defined in number order
	 * |      |
	 * |      |
	 * ========
	 * 4      3
	 */
	sf::VertexArray vertexArray(sf::PrimitiveType::Quads);
	std::cout << "Application started\n";
	while (window.isOpen()) {

		elapsedTime += timer.restart().asSeconds();

		sf::Event ev;
		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case sf::Event::Closed:
				window.close();

			case sf::Event::MouseWheelScrolled:

				if (ev.mouseWheelScroll.delta > 0) {
					camera.zoom(0.95);
					window.setView(camera);
				}
				else if (ev.mouseWheelScroll.delta < 0) {
					camera.zoom(1.05);
					window.setView(camera);
				}

			case sf::Event::KeyPressed:

				if (ev.key.code == sf::Keyboard::Key::A) {
					camera.move(sf::Vector2f(-SQUARE_SIZE, 0));
					window.setView(camera);
				}
				else if (ev.key.code == sf::Keyboard::Key::D) {
					camera.move(sf::Vector2f(SQUARE_SIZE, 0));
					window.setView(camera);
				}
				else if (ev.key.code == sf::Keyboard::Key::W) {
					camera.move(sf::Vector2f(0, -SQUARE_SIZE));
					window.setView(camera);
				}
				else if (ev.key.code == sf::Keyboard::Key::S) {
					camera.move(sf::Vector2f(0, SQUARE_SIZE));
					window.setView(camera);
				}
			}
		}

		if (elapsedTime > SECONDS_PER_UPDATE) {
			elapsedTime -= SECONDS_PER_UPDATE;


			/*
			 * In each update:
			 *  1. Create/update neighbors matrix
			 *     - Count neighbors of alive cells and neighbors of bordering cells
			 *  2. Run conway rules
			 *     - Cells with < 2 or > 3 neighbors die
			 *     - Otherwise, live
			 *     - Revive dead cells based on neighbor matrix
			 *       - If a dead cell has 3 neighbors, revive it
			 */
			updateNeighborMatrix(board, neighborsMatrix);
			conway(board, neighborsMatrix);
			vertexArray.clear();

			// Drawing ONLY
			for(auto it = board.begin(); it != board.end(); it++) {
				int x = it->first.first;
				int y = it->first.second;


				sf::Vertex topLeft(
					sf::Vector2f(x * SQUARE_SIZE, y * SQUARE_SIZE),
					sf::Color::White
				);
				sf::Vertex topRight(
					sf::Vector2f((x + 1) * SQUARE_SIZE, y * SQUARE_SIZE),
					sf::Color::White
				);
				sf::Vertex bottomRight(
					sf::Vector2f((x + 1) * SQUARE_SIZE, (y + 1) * SQUARE_SIZE),
					sf::Color::White
				);
				sf::Vertex bottomLeft(
					sf::Vector2f(x * SQUARE_SIZE, (y + 1) * SQUARE_SIZE),
					sf::Color::White
				);

				vertexArray.append(topLeft);
				vertexArray.append(topRight);
				vertexArray.append(bottomRight);
				vertexArray.append(bottomLeft);

			}

			/* Draw Neighbors
			for (auto it = neighborsMatrix.begin(); it != neighborsMatrix.end(); it++) {
				int x = it->first.first;
				int y = it->first.second;


				sf::Vertex topLeft(
					sf::Vector2f(x * SQUARE_SIZE, y * SQUARE_SIZE),
					sf::Color::Red
				);
				sf::Vertex topRight(
					sf::Vector2f((x + 1) * SQUARE_SIZE, y * SQUARE_SIZE),
					sf::Color::Red
				);
				sf::Vertex bottomRight(
					sf::Vector2f((x + 1) * SQUARE_SIZE, (y + 1) * SQUARE_SIZE),
					sf::Color::Red
				);
				sf::Vertex bottomLeft(
					sf::Vector2f(x * SQUARE_SIZE, (y + 1) * SQUARE_SIZE),
					sf::Color::Red
				);

				vertexArray.append(topLeft);
				vertexArray.append(topRight);
				vertexArray.append(bottomRight);
				vertexArray.append(bottomLeft);
			}
			*/

		}

		window.clear(sf::Color::Black);
		window.draw(vertexArray);
		window.display();
	}

	return 0;
}