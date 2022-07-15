#include "App.h"


App::App(unsigned width, unsigned height, std::string title)
	: WIDTH(width),
	HEIGHT(height),
	window(sf::VideoMode(WIDTH, HEIGHT), title)
{
	init();
}

void App::init() {
	circle.setRadius(100.f);
	circle.setFillColor(sf::Color(90, 90, 90));

	zoom = 1.f;
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
			break;
		
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				trackMouse = true;
				previousMousePosition = sf::Mouse::getPosition(window);
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				trackMouse = false;
			}
			break;

		case sf::Event::MouseWheelScrolled:

			if (ev.mouseWheelScroll.delta > 0) {
				camera.zoom(0.95f);
			}
			else if (ev.mouseWheelScroll.delta < 0) {
				camera.zoom(1.05f);
			}

			break;

		case sf::Event::Resized:
			printf("TODO: HANDLE RESIZE\n");
			break;
		}
	}
}

void App::update() {

	if (trackMouse) {
		sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
		sf::Vector2i delta = currentMousePosition - previousMousePosition;
		
		/*
		 * Camera movement should function the same when using
		 * left click + drag no matter the zoom level. Scale the
		 * mouse position delta by viewSize / WIDTH to get a decent
		 * scale factor. This makes the left click + drag feel alright
		 * at any zoom level.
		 */
		sf::Vector2f viewSize = camera.getSize();
		float scale = viewSize.x / WIDTH;

		camera.move(-delta.x * scale, -delta.y * scale);

		previousMousePosition = currentMousePosition;
	}

}

void App::render() {

	window.setView(camera);
	window.clear();

	window.draw(circle);
	window.display();
}
