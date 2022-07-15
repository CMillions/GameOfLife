#include "App.h"


App::App(unsigned width, unsigned height, std::string title)
	: WIDTH(width),
	HEIGHT(height),
	window(sf::VideoMode(WIDTH, HEIGHT), title)
{
	init();
}

void App::init() {
	
}

void App::setFramerateLimit(unsigned fps) {
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(fps);
}

void App::start() {
	loop();
}

void App::loop() {

	while (window.isOpen()) {
		processEvents();
		update();
		render();
	}
}

void App::processEvents() {
	sf::Event ev;
	while (window.pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window.close();
		}
	}
}

void App::update() {

}

void App::render() {
	window.clear();

	window.display();
}
