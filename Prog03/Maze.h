#pragma once
#include "wx/wx.h"
#include <iostream>
#include <fstream>	// access to filestream I/O functionality in C++
#include <string>	// strings might come in handy, but it's up to you
// wxWidgets uses wxString variables, do some searching on how to 
// convert between the two if you need to use strings!
using namespace std;

class Maze
{
public:
	//-----------------------------------------------------------------
	// Maze constructor
	// Reads in the map.
	// Sets valid to true if there are no invalid characters found and
	// the size of the map is valid
	//-----------------------------------------------------------------
	Maze(wxFrame* parent, ifstream& ifs);

	// destructor - cleans up ALL dynamically allocated memory
	~Maze();

	// private class attributes
private:
	// some static constants, map characters, size of cell to display, size of map, etc.
	static const int CELLSIZE = 20;
	static const int MAXSIZE = 30;
	static const char OPEN = 'O';
	static const char DEADEND = '+';
	static const char EXIT = 'E';
	static const char START = 'S';
	static const char VISITED = 'X';
	int width, height;	// width and height of the maze
	bool free;			// Did RecSolve reach the exit?
	bool valid;			// Is Maze Valid?
	wxFrame* panel = nullptr;

	// private functions (outside world CAN NOT use)
private:
	// You will need to dynamically allocate the 2D arrays below like
	// we discussed in class – keep in mind, once the arrays are
	// correctly allocated, they may be used exactly like statically 
	// allocated arrays (orig[row][col])
	char** orig;	// original maze as read in from data file
	char** solved;	// updated maze used to find exit (visited cells marked)

	// pointer to the maze we want to show the user - you do not need to allocate
	// any memory for maze2Show, it is simply for convenience - it is always equal to 
	// either orig or solved.
	// - call ShowOriginal() or ShowSolved() to change which one is displayed
	char** maze2Show;

	//-----------------------------------------------------------------
	// Solves the maze recursively.
	// First, as a base case, it checks if the current cell is the exit.
	// If the cell is not the exit, it then checks if any adjacent cells
	// are the exit if the exit is adjacent, RecSolve is called with
	// the location of the exit as parameters.
	// Otherwise, it checks for adjacent open cells and makes calls
	// to RecSolve to move to said open cells.
	//-----------------------------------------------------------------
	void RecSolve(int row, int col);

	// public functions (outside world CAN use / public interface)
public:
	//-----------------------------------------------------------------
	// Returns true if the maze is valid, false otherwise
	//-----------------------------------------------------------------
	bool IsValid() const { return valid; }

	//-----------------------------------------------------------------
	// Returns true if the exit of the maze was found, false otherwise
	//-----------------------------------------------------------------
	bool IsFree() const { return free; }

	//-----------------------------------------------------------------
	// First checks if the mouse clicked a deadend or the exit and
	// returns if either is true.
	// Otherwise, it establishes the starting cell, colors it red,
	// and calls RecSolve with the row and column of the clicked
	// cell as parameters.
	//-----------------------------------------------------------------
	void Solve(int xPixel, int yPixel);

	//-----------------------------------------------------------------
	// Calls Show with the original maze 2d array as parameter
	//-----------------------------------------------------------------
	//void ShowOriginal(wxPaintDC& dc) { Show(dc, orig); }
	void ShowOriginal() { maze2Show = orig; }

	//-----------------------------------------------------------------
	// Calls Show with the solved maze 2d array as parameter
	//-----------------------------------------------------------------
	void ShowSolved() { maze2Show = solved; }

	//-----------------------------------------------------------------
	// Simply copies original maze data over solution maze
	//-----------------------------------------------------------------
	void ResetSolution();

	//-----------------------------------------------------------------
	// Displays the 2D array pointed to by maze2Show - a device context
	// must be passed to the function so that drawing can take place
	// This is where you code the drawing of your original maze and 
	// the solved maze
	//-----------------------------------------------------------------
	void Show(wxPaintDC& dc);
};

