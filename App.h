#pragma once

#include <SFML/Graphics.hpp>

class App {
public:
	App(unsigned width, unsigned height, std::string title);
	void start();
	void setFramerateLimit(unsigned fps);

private:

	const unsigned int WIDTH;
	const unsigned int HEIGHT;

	sf::RenderWindow window;
	sf::View camera;

	void init();
	void loop();
	
	void processEvents();
	void update();	
	void render();



	
};