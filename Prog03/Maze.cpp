#include "Maze.h"		// Maze class specification

Maze::~Maze()
{
	// Now, delete our 2D arrays!
	if (orig != nullptr)
	{
		// how to delete a dynamically allocated 2D array?
	}
	if (solved != nullptr)
	{
		// same here :)
	}
}

Maze::Maze(wxFrame* parent, ifstream& ifs)
{
	// intialize everything here
	// *************************
	// initially, the maze has not been solved, so set free to false
	free = false;
	// initially, the maze is invalid (we haven't read anything yet)
	valid = false;

	// set the panel to the passed in frame
	panel = parent;		// a pointer to the parent GUI element is often useful
	orig = nullptr;
	solved = nullptr;
	maze2Show = orig;



	// several important things need to happen in the constructor
	// You need to dynamically allocate both 2D arrays (orig and solved)

	orig = new char*[height];
	for (int i = 0; i < height; i++) {
		orig[i] = new char[width];
	}

	solved = new char*[height];
	for (int i = 0; i < height; i++) {
		solved[i] = new char[width];
	} 
	
	// flip width and height?
	// use member variables

	// once orig has been set, don't change it - it contains the original
	// maze data! the solved 2D array can be modified to include information
	// about our starting point (where the user clicked) and what cells we
	// visited while finding the exit in our maze (colored blue)

	// you will also need to read from the ifstream passed to this constructor
	// read in all the data to the orig 2D array and determine whether this
	// is a valid maze file or not! (if it is valid, don't forget valid = true;)
	// it will be helpful to set width and height (attributes of the Maze class) 
	wxMessageDialog* invalid = new wxMessageDialog(nullptr,
		wxT("Invalid maze file provided."), wxT("Error"), wxOK);


	wxMessageDialog* youarehere = new wxMessageDialog(nullptr,
		wxT("YOU ARE HERE"), wxT("YOU ARE HERE"), wxOK);
	wxMessageDialog* toobig = new wxMessageDialog(nullptr,
		wxT("YOU ARE HERE"), wxT("YOU ARE HERE"), wxOK);

	ifs >> width >> height;
	if (width > MAXSIZE || height > MAXSIZE) {
		toobig->ShowModal();
		return; // invalid
	}

	for (int i = 0; i < height; i++) {
		string mazeChars;
		ifs >> mazeChars;

		for (int j = 0; j < width; j++) {
			if ((mazeChars.at(j) != OPEN) && (mazeChars.at(j) != DEADEND)
				&& (mazeChars.at(j) != EXIT)) 
			{
				invalid->ShowModal();
				return; // invalid
			}
			youarehere->ShowModal();
			orig[i][j] = mazeChars.at(j);
			solved[i][j] = mazeChars.at(j);
		}

	}
	valid = true;

	// For now, select the original maze to show
	// *NOTE: this function doesn't draw anything, it just sets the maze2Show
	// pointer to the orig 2D array (it already should be, but just being cautious)
	delete invalid;
	ShowOriginal();
}

void Maze::Solve(int xPixel, int yPixel)
{
	// You will need to setup the solution environment and then call RecSolve,
	// your recursive maze solver function

	// As part of your setup, how to we map an (x, y) pixel coordinate to one
	// of the cells on our maze? We want to pass RecSolve() the row and col of the
	// cell the user clicked on!

	// force a full re-draw on the cMain wxFrame
	panel->Refresh();
	panel->Update();
}

void Maze::RecSolve(int row, int col)
{
	// You will need some base cases and some recursive cases here!
	// I leave it to you to think this through :)

	// base case: check if current cell is exit
	if (solved[row][col] == EXIT) {
		free = true;
		wxMessageDialog* clickedExit = new wxMessageDialog(nullptr,
			wxT("You clicked on the exit!"), wxT("Exit found!"), wxOK);
		clickedExit->ShowModal();
		delete clickedExit;
		return;
	}
	// other base case: current cell is deadend
	if (solved[row][col] == DEADEND) {
		wxMessageDialog* clickedDeadEnd = new wxMessageDialog(nullptr,
			wxT("You clicked on a dead end!"), wxT("Dead end!"), wxOK);
		clickedDeadEnd->ShowModal();
		delete clickedDeadEnd;
		return;
	}
	// check adjacent cells
	if (solved[row - 1][col] == EXIT) { // going up
		RecSolve(--row, col);
		free = true;
	}
	if (solved[row + 1][col] == EXIT) { // going down
		RecSolve(++row, col);
		free = true;
	}
	if (solved[row][col - 1] == EXIT) { // going left
		RecSolve(row, --col);
		free = true;
	}
	if (solved[row][col + 1] == EXIT) { // going right
	{
		RecSolve(row, ++col);
		free = true;
	}
	/*if (free == true) {
		wxMessageDialog* foundExit = new wxMessageDialog(nullptr,
			wxT("Exit was found!"), wxT("Exit found!"), wxOK);
		foundExit->ShowModal();
		delete foundExit;
	}*/
	solved[row][col] = VISITED;
	RecSolve(row-1, col); // going up
	RecSolve(row+1, col); // going down
	RecSolve(row, col-1); // going left
	RecSolve(row, col+1); // going right




		// Again, don't change the orig 2D array, we always want a copy of the
		// original maze data - change the solved array to indicate the starting
		// point and all the cells visited while searching for the exit
	}
}

// Copy over the solved array with the orig data
void Maze::ResetSolution()
{
	if ((orig != nullptr) && (solved != nullptr))
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				solved[i][j] = orig[i][j];
			}
	}
}

void Maze::Show(wxPaintDC& dc)
{
	// if the maze to show doesn't exist, then return
	if (maze2Show == nullptr) return;

	// Loop through rows 
	for (int i = 0; i < height; i++)
	{
		// Loop through columns
		for (int j = 0; j < width; j++)
		{
			// get the current maze cell
			char square = maze2Show[i][j];
			
			dc.SetPen(*wxBLACK_PEN);

			switch (square) {
			case OPEN:
				dc.SetBrush(*wxWHITE_BRUSH);
				dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
				break;
			case DEADEND:
				dc.SetBrush(*wxBLACK_BRUSH);
				dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
				break;
			case EXIT:
				dc.SetBrush(*wxGREEN_BRUSH);
				dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
				break;
			case START:
				dc.SetBrush(*wxRED_BRUSH);
				dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
				break;
			case VISITED:
				dc.SetBrush(*wxBLUE_BRUSH);
				dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
				break;
			default:
				valid = false;
			}

			if (!valid)
				break;

			// the rest is up to you!
			// for every cell, you should draw a colored rectangle to 
			// wxFrame via the "dc" devicee context variable
			// I suggest looking into the DrawRectangle function			
		}
		if (!valid)
			break;
	}
}
