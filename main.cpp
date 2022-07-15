#include "App.h"

int main() {

	const unsigned int WIDTH = 1280u;
	const unsigned int HEIGHT = 720u;

	App app(WIDTH, HEIGHT, "Game of Life");
	app.setFramerateLimit(60);
	app.setUpdateLimit(4);
	app.start();

	return 0;
}