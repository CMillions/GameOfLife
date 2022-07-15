#include "App.h"


App::App(unsigned width, unsigned height, std::string title)
	: windowWidth(width),
	windowHeight(height),
	SQUARE_SIZE(32),
	window(sf::VideoMode(windowWidth, windowHeight), title)
{
	init();
}

void App::init() {
	const int INITIAL_CELLS_ALIVE = 1000;

	zoom = 1.f;
	camera.setSize(windowWidth, windowHeight);
	conwayVertexArray.setPrimitiveType(sf::Quads);
}

void App::setFramerateLimit(unsigned fps) {
	window.setFramerateLimit(fps);
}

void App::setUpdateLimit(float ups) {
	updatesPerSecond = ups;
	secondsPerUpdate = 1.f / ups;
}

void App::start() {
	loop();
}

void App::loop() {

	float elapsedTime = 0.f;

	while (window.isOpen()) {
		processEvents();
		updateMouse();

		elapsedTime += timer.restart().asSeconds();
		if (elapsedTime > secondsPerUpdate) {
			elapsedTime -= secondsPerUpdate;
			update();
		}

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
			{
				bool zoomedIn = ev.mouseWheelScroll.delta > 0;

				if (zoomedIn) {
					camera.zoom(0.95f);
					zoom *= 0.95f;
				}
				else if (ev.mouseWheelScroll.delta < 0) {
					camera.zoom(1.05f);
					zoom *= 1.05f;
				}
			}

			break;

		case sf::Event::Resized:

			// Update camera view size and set it to
			// the current zoom level. Allows window to
			// be resized and keep current view roughly
			// the same.
			windowWidth = ev.size.width;
			windowHeight = ev.size.height;
			camera.setSize(sf::Vector2f(windowWidth, windowHeight));
			camera.zoom(zoom);

			break;
		}
	}
}

void App::update() {

	conwayEngine->nextGeneration();
}

void App::updateMouse() {
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
		float scaleX = viewSize.x / windowWidth;
		float scaleY = viewSize.y / windowHeight;

		camera.move(-delta.x * scaleX, -delta.y * scaleY);

		previousMousePosition = currentMousePosition;
	}
}

void App::render() {
	createVertexArrays();
	window.setView(camera);

	window.clear(sf::Color::Black);

	window.draw(conwayVertexArray);
	
	window.display();
}

void App::createVertexArrays() {
	auto conwayBoard = conwayEngine->currentGeneration();

	conwayVertexArray.clear();
	for (auto it = conwayBoard.begin(); it != conwayBoard.end(); it++) {
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

		conwayVertexArray.append(topLeft);
		conwayVertexArray.append(topRight);
		conwayVertexArray.append(bottomRight);
		conwayVertexArray.append(bottomLeft);
	}
}
