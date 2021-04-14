#include <SFML/Graphics.hpp>
#include <memory>

#include "GameOfLifeBoard.h"

class App
{
private:

	std::unique_ptr
		<sf::RenderWindow> m_window; // A pointer to the window object
	unsigned int m_width;			 // The width of the window
	unsigned int m_height;           // The height of the window
	std::string m_title;             // The title of the window

	const unsigned int FPS;			 // The desired framerate in frames per second (FPS)
	bool m_running;					 // Is the app running?

	sf::Vector2i m_mousePos;		 // The position of the mouse

	sf::Clock m_clock;				 // Clock used for delta time
	float m_elapsedTime;			 // Checks elapsedTime against an update time

	sf::Event m_event;				 // Holds the event that is currently happening

	sf::VertexArray m_rowLines;      // Has vertices for the horizontal lines to make the cells easily visible
	sf::VertexArray m_columnLines;   // Has vertices for the vertical lines to make the cells easily visible
	sf::VertexArray m_cellVertices;  // Has vertices for each cell. Is colored black or white to represent state

	GameOfLifeBoard m_board;		 // Instance of the game board for Conway's Game of Life
	bool m_simulating;				 // Is the simulation running?
	bool m_menuShowing;				 // Is the menu showing?

private: // Private member functions

	void init();
	void loop();

	void draw();

	void handleEvents();
	void handleKeyPress(const sf::Event &ev);
	void handleMouseButtonPress(const sf::Event& ev);

	int convertGridPosToQuadVertex(sf::Vector2i gridPos);
	sf::Vector2i convertMousePosToGridPos();

	void updateCellVertices(unsigned int row, unsigned int col);

public: // Public member functions

	App();
	void launch();

};

