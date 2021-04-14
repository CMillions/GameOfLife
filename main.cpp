/* File:   main.cpp
 * Author: Connor D
 * Date:   April 13, 2021
 * 
 * This program is an implementation of Conway's game of life. Although
 * the board size is usually infinite, this uses a fixed board size that
 * can be changed by the programmer. The user can pause and reset the
 * simulation at any time. The initial conditions can only be changed
 * when the simulation is at age zero.
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include "App.h"

#define DEBUG

int main()
{
	std::cout << "Beginning program execution" << std::endl;

	App myApp;	
	myApp.launch();

	std::cout << "Ending program execution" << std::endl;


	return 0;
}