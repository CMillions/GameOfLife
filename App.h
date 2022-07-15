#pragma once


#include <SFML/Graphics.hpp>
#include "ConwayEngine.h"

#include <memory>

class App {
private:

	unsigned int windowWidth;
	unsigned int windowHeight;

	sf::Clock timer;
	float updatesPerSecond;
	float secondsPerUpdate;

	sf::RenderWindow window;
	sf::View camera;

	sf::Vector2i previousMousePosition;
	bool trackMouse;
	float zoom;

	std::unique_ptr<ConwayEngine> conwayEngine = std::make_unique<ConwayEngine>(1000,1);

	// Drawing the squares
	const int SQUARE_SIZE;
	sf::VertexArray conwayVertexArray;

private:
	void init();
	void loop();
	
	void processEvents();
	void update();	
	void updateMouse();
	void render();
	
	void createVertexArrays();

public:
	App(unsigned width, unsigned height, std::string title);
	void start();
	void setFramerateLimit(unsigned fps);
	void setUpdateLimit(float ups);
};