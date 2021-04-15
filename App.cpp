#include "App.h"

const unsigned int NUM_ROWS = 50;
const unsigned int NUM_COLS = 50;

App::App() : FPS(60u), m_rowLines(sf::Lines), m_columnLines(sf::Lines), m_cellVertices(sf::Quads),
			 m_board(NUM_ROWS, NUM_COLS)
{
	m_width = 800;
	m_height = 800;
	m_title = "Conway's Game of Life";

	m_elapsedTime = 0;

	// Create the window as a unique pointer, force framerate limit
	// make sure VSYNC is disabled
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_width, m_height), m_title);
	m_window->setFramerateLimit(FPS);
	m_window->setVerticalSyncEnabled(false);

	// Load the font for the application
	m_font = std::make_unique<sf::Font>();
	m_font->loadFromFile("resources/fonts/arial.ttf");

	for (auto& textObj : m_text)
	{
		textObj.setFont(*m_font);
		textObj.setCharacterSize(16);
		textObj.setFillColor(sf::Color::White);
	}

	m_running = true;     // the application is running by default
	
	m_simulating = false; // The simulation is paused by default
}


void App::init()
{	
	initVertices();
	initMenu();
}

void App::initVertices()
{
	const sf::Color GRIDLINE_COLOR(120, 120, 120, 125);

	float quadWidth = m_width / NUM_ROWS;
	float quadHeight = m_height / NUM_COLS;

	// Calculate the number of cells on the board and
	// allocate enough space for the vertices
	int numCells = NUM_ROWS * NUM_COLS;
	m_cellVertices.resize(4 * numCells);

	// If we want to draw lines to separate things, we need
	// two points for each line
	int numHorizontalLinesNeeded = 2 * NUM_ROWS;
	int numVerticalLinesNeeded = 2 * NUM_COLS;

	// Allocate enough space for the required number
	// of vertices
	m_rowLines.resize(numHorizontalLinesNeeded);
	m_columnLines.resize(numVerticalLinesNeeded);


	float xPos = 0, yPos = 0;

	// Set the vertices for each line every iteration
	for (int i = 0; i < numVerticalLinesNeeded; i += 2)
	{
		// The vertical lines have constant y positions but changing
		// x positions
		xPos = static_cast<float>(i / 2 * m_width / NUM_COLS);

		m_columnLines[i].position = sf::Vector2f(xPos, 0.f);
		m_columnLines[i + 1].position = sf::Vector2f(xPos, m_height);

		m_columnLines[i].color = GRIDLINE_COLOR;
		m_columnLines[i + 1].color = GRIDLINE_COLOR;
	}

	// Set the vertices for each line every iteration
	for (int i = 0; i < numHorizontalLinesNeeded; i += 2)
	{
		// The horizontal lines have constant x positions but changing
		// y positions
		yPos = static_cast<float>(i / 2 * m_height / NUM_ROWS);

		m_rowLines[i].position = sf::Vector2f(0.f, yPos);
		m_rowLines[i + 1].position = sf::Vector2f(m_width, yPos);

		m_rowLines[i].color = GRIDLINE_COLOR;
		m_rowLines[i + 1].color = GRIDLINE_COLOR;
	}

	/* Set the vertices for every cell on the board
	 * Requires a 'row' and 'col' variable to calculate
	 * the exact placement of the vertices
	 */
	int row = 0, col = 0;
	for (int i = 0; i < 4 * numCells; i += 4, col++)
	{
		// If the column number is too large,
		// move down a row and reset column to zero
		if (col >= NUM_COLS)
		{
			row++;
			col = 0;
		}

		// Set the position of the four corner vertices of the quad
		m_cellVertices[i].position = sf::Vector2f(col * quadWidth, (row + 1) * quadHeight);
		m_cellVertices[i + 1].position = sf::Vector2f(col * quadWidth, row * quadHeight);
		m_cellVertices[i + 2].position = sf::Vector2f((col + 1) * quadWidth, row * quadHeight);
		m_cellVertices[i + 3].position = sf::Vector2f((col + 1) * quadWidth, (row + 1) * quadHeight);

		int test = rand() % 10;

		if (test > 6)
			m_board.reviveCell(row, col);

		updateCellVertices(row, col);
	}
}

void App::initMenu()
{
	m_menuShowing = true; // show the menu by default

	m_menuRect.setSize(sf::Vector2f(350.f, 300.f));
	m_menuRect.setPosition(0, 0);
	m_menuRect.setFillColor(sf::Color(64, 64, 64, 240));
	m_menuRect.setOutlineColor(sf::Color::Green);
	m_menuRect.setOutlineThickness(1.f);

	m_text[0].setPosition(10, 10);
	m_text[0].setString("Info & Controls");

	m_text[1].setPosition(10, 90);
	m_text[1].setString("Generation: " + std::to_string(m_board.age()) + ""\
						"\n\nSPACE - Pause/unpause simulation"\
						"\n\nR - Return board to generation 0"\
						"\n\nC - Clear board"\
						"\n\nESC - Hide/unhide menu"\
						);
}

void App::loop()
{
	const float UPDATE_TIME = 0.50f;

	while (m_running)
	{
		handleEvents();

		if (m_simulating && m_elapsedTime >= UPDATE_TIME)
		{
			m_elapsedTime = 0.0f;

			m_board.nextState();

			m_text[1].setString("Generation: " + std::to_string(m_board.age()) + ""\
				"\n\nSPACE - Pause/unpause simulation"\
				"\n\nR - Return board to generation 0"\
				"\n\nC - Clear board"\
				"\n\nESC - Hide/unhide menu"\
			);

			updateAllCellVertices();
		}
		else if (!m_simulating && (m_board.age() == 0) && (m_elapsedTime >= UPDATE_TIME))
		{
			m_elapsedTime = 0.0f;
			m_mousePos = sf::Mouse::getPosition(*m_window); // Gets the mouse position is (x,y) inside the window
		}

		m_elapsedTime += m_clock.restart().asSeconds();

		draw();
	}

	m_window->close(); // Close the window and return to main
}

void App::draw()
{
	m_window->clear(sf::Color::Black);
	
	m_window->draw(m_cellVertices);
	m_window->draw(m_columnLines);
	m_window->draw(m_rowLines);

	if (m_menuShowing)
	{
		m_window->draw(m_menuRect);
		m_window->draw(m_text[0]);
		m_window->draw(m_text[1]);
	}

	m_window->display();
}

void App::handleEvents()
{
	while (m_window->pollEvent(m_event))
	{
		switch (m_event.type)
		{
		case sf::Event::Closed:
			m_running = false;
			break;

		case sf::Event::KeyPressed:
			handleKeyPress(m_event);
			break;

		case sf::Event::MouseButtonPressed:
			handleMouseButtonPress(m_event);
			break;

		default:
			break;
		}
	}
}

void App::handleKeyPress(const sf::Event& ev)
{
	// Checks the value of 'ev.key.code' to decide
	// what to do

	switch (ev.key.code)
	{
	case sf::Keyboard::F4: // end the program
		m_running = false;
		break;
		
	case sf::Keyboard::Escape:
		m_menuShowing = !m_menuShowing; // Toggle the menu on or off
		break;

	case sf::Keyboard::Space:
		m_simulating = !m_simulating;   // Toggle the simulation on or off
		break;

	case sf::Keyboard::R:			    // Pause and reset board to initial state
		m_simulating = false;
		m_board.reset();
		m_text[1].setString("Generation: " + std::to_string(m_board.age()) + ""\
			"\n\nSPACE - Pause/unpause simulation"\
			"\n\nR - Return board to generation 0"\
			"\n\nC - Clear board"\
			"\n\nESC - Hide/unhide menu"\
		);

		updateAllCellVertices();
		break;
	}

}

void App::handleMouseButtonPress(const sf::Event& ev)
{

}

int App::convertGridPosToQuadVertex(sf::Vector2i gridPos)
{
	/* Now with grid coordinates of the square the mouse
	 * is in, convert it to the bottom-left corner
	 * of the current square. The drawing below shows
	 * the vertex numbering.
	 *
	 *      1 ---- 2
	 *        |  |
	 *      0 ---- 4
	 */

	int pixelIndex = (gridPos.y * NUM_ROWS + gridPos.x) * 4;

	return pixelIndex;
}

sf::Vector2i App::convertMousePosToGridPos()
{
	sf::Vector2i squareIndex;

	float squareWidth = static_cast<float>(m_window->getSize().x / NUM_ROWS);
	float squareHeight = static_cast<float>(m_window->getSize().y / NUM_COLS);

	squareIndex.x = m_mousePos.x / squareWidth;
	squareIndex.y = m_mousePos.y / squareHeight;

	return squareIndex;
}

void App::updateCellVertices(unsigned int row, unsigned int col)
{
	int bottomLeftVertex = convertGridPosToQuadVertex(sf::Vector2i(row, col));

	for (int i = bottomLeftVertex; i < bottomLeftVertex + 4; i++)
	{
		if (m_board.isCellAlive(row, col))
		{
			m_cellVertices[i].color = sf::Color::White;
		}
		else
			m_cellVertices[i].color = sf::Color::Black;
	}
}

void App::updateAllCellVertices()
{
	for (unsigned int row = 0; row < m_board.rows(); row++)
	{
		for (unsigned int col = 0; col < m_board.cols(); col++)
		{
			updateCellVertices(row, col);
		}
	}
}


void App::launch()
{
	init();
	loop();
}
