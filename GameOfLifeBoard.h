/* File:   GameOfLifeBoard.h
 * Author: Connor D
 * Date:   04/07/2021
 *
 * Description: Board class for Conway's Game of Life.
 */

#ifndef GAME_OF_LIFE_BOARD_H
#define GAME_OF_LIFE_BOARD_H

#include <vector>
#include <algorithm>
#include <stdlib.h>

constexpr auto DEFAULT_ROWS = 10u;
constexpr auto DEFAULT_COLS = 10u;

class GameOfLifeBoard
{
public: // Constructor & destructor

	GameOfLifeBoard()
	{
		// Initialize enough room for the rows
		setSize(DEFAULT_ROWS, DEFAULT_COLS);

		m_age = 0;
	}

	GameOfLifeBoard(unsigned int rows, unsigned int cols)
	{
		setSize(rows, cols);

		m_age = 0;
	}

public: // Member functions

	const bool isCellAlive(unsigned int row, unsigned int col)
		const
	{
		if (row < m_rows && col < m_cols)
			return m_board[row][col];

		return false;
	}

	void killCell(unsigned int row, unsigned int col)
	{
		if (row < m_rows && col < m_cols)
		{
			m_initialBoard[row][col] = false;
			m_board[row][col] = false;
		}
	}

	void reviveCell(unsigned int row, unsigned int col)
	{
		if (row < m_rows && col < m_cols)
		{
			m_initialBoard[row][col] = true;
			m_board[row][col] = true;
		}
	}

	void clear()
	{
		for (auto& row : m_initialBoard)
		{
			for (auto cell : row)
				cell = false;
		}

		m_board = m_initialBoard;
	}

	// THIS WILL ERASE THE BOARD AND RESIZE IT
	void setSize(unsigned int rows, unsigned int cols)
	{
		// Update member variables
		setRows(rows);
		setCols(cols);

		// Resize the board
		m_board.resize(m_rows, std::vector<bool>(m_cols, false));
		m_initialBoard.resize(m_rows, std::vector<bool>(m_cols, false));

		// Ensure every cell is false
		for (auto& row : m_board)
			for (auto cell : row)
				cell = false;

		for (auto& row : m_initialBoard)
			for (auto cell : row)
				cell = false;
	}

	void nextState()
	{
		if (m_age == 0)
			m_board = m_initialBoard;

		// Not space efficient, especially for large
		// borad sizes. O(m*n) space complexity,
		// can maybe be achieved faster with bit sets?
		std::vector<std::vector<bool> > tempBoard;
		tempBoard.resize(m_rows, std::vector<bool>(m_cols));

		// Determine the next state of the board
		for (int row = 0; row < tempBoard.size(); row++)
		{
			for (int col = 0; col < tempBoard[row].size(); col++)
			{
				tempBoard[row][col] = updateCell(row, col);
			}
		}

		// Assign the current board to the temporary board
		m_board = tempBoard;
		m_age++;
	}

	void reset()
	{
		m_age = 0;
		m_board = m_initialBoard;
	}

	void initializeToRandomState()
	{
		m_age = 0;

		for (int i = 0; i < 100; i++)
		{
			int rowIndex = rand() % m_rows;
			int colIndex = rand() % m_cols;
			int randomNumber = rand() % 100;

			// 25% chance to happen
			if (randomNumber >= 95)
			{
				m_board[(rowIndex - 1) % m_rows][colIndex] = true;
				m_board[rowIndex][colIndex] = true;
				m_board[(rowIndex + 1) % m_rows][colIndex] = true;

				m_board[(rowIndex - 1) % m_rows][(colIndex - 1) % m_cols] = true;
				m_board[(rowIndex + 1) % m_rows][(colIndex + 1) % m_cols] = true;
			}
			else if (randomNumber >= 25)
			{
				m_board[rowIndex][colIndex] = true;
				m_board[(rowIndex - 1) % m_rows][(colIndex - 1) % m_cols] = true;
			}
		}
	}

	const unsigned int rows() const { return m_rows; }
	const unsigned int cols() const { return m_cols; }
	const unsigned int age()  const { return m_age; }


private: // Member functions

	void setRows(unsigned int rows) { m_rows = rows; }
	void setCols(unsigned int cols) { m_cols = cols; }

	unsigned int countNeighbors(unsigned int row, unsigned int col)
	{
		int numNeighbors = 0;

		int prevRow = row - 1;
		int nextRow = row + 1;

		int prevCol = col - 1;
		int nextCol = col + 1;

		if (prevRow >= 0 && prevCol >= 0)
			numNeighbors += m_board[prevRow][prevCol];

		if (prevCol >= 0)
			numNeighbors += m_board[row][prevCol];

		if (nextRow < m_rows && prevCol >= 0)
			numNeighbors += m_board[nextRow][prevCol];

		if (prevRow >= 0)
			numNeighbors += m_board[prevRow][col];

		if (nextRow < m_rows)
			numNeighbors += m_board[nextRow][col];

		if (prevRow >= 0 && nextCol < m_cols)
			numNeighbors += m_board[prevRow][nextCol];

		if (nextCol < m_cols)
			numNeighbors += m_board[row][nextCol];

		if (nextRow < m_rows && nextCol < m_cols)
			numNeighbors += m_board[nextRow][nextCol];

		return numNeighbors;
	}

	bool updateCell(unsigned int row, unsigned int col)
	{
		unsigned int neighbors = countNeighbors(row, col);

		if (m_board[row][col])
		{
			if (neighbors == 2 || neighbors == 3)
				return true;
		}
		else
		{
			if (neighbors == 3)
				return true;
		}

		return false;
	}

private: // Member variables

	unsigned int m_rows;
	unsigned int m_cols;

	unsigned int m_age;

	std::vector<std::vector<bool> > m_board;
	std::vector<std::vector<bool> > m_initialBoard;

};

#endif