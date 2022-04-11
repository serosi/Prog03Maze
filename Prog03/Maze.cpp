#include "Maze.h"		// Maze class specification
#include "cMain.h"

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

	// once orig has been set, don't change it - it contains the original
	// maze data! the solved 2D array can be modified to include information
	// about our starting point (where the user clicked) and what cells we
	// visited while finding the exit in our maze (colored blue)

	// you will also need to read from the ifstream passed to this constructor
	// read in all the data to the orig 2D array and determine whether this
	// is a valid maze file or not! (if it is valid, don't forget valid = true;)
	// it will be helpful to set width and height (attributes of the Maze class) 
	wxMessageDialog* invalid = new wxMessageDialog(nullptr,
		wxT("The provided maze file contains invalid characters."), wxT("Maze Error"), wxOK);
	wxMessageDialog* tooBig = new wxMessageDialog(nullptr,
		wxT("The provided maze file is greater than 30x30 cells."), wxT("Size Error"), wxOK);

	ifs >> width >> height;
	if (width > MAXSIZE || height > MAXSIZE) {
		tooBig->ShowModal();
		return; // invalid
	}

	orig = new char* [height];
	for (int i = 0; i < height; i++) {
		orig[i] = new char[width];
	}

	solved = new char* [height];
	for (int i = 0; i < height; i++) {
		solved[i] = new char[width];
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

	ShowSolved();
	free = false;

	if (orig[yPixel / CELLSIZE][xPixel / CELLSIZE] == EXIT) {
		wxMessageDialog* clickedExit = new wxMessageDialog(nullptr,
			wxT("You clicked on the exit!"), wxT("Exit found!"), wxOK);
		clickedExit->ShowModal();
		delete clickedExit;
		return;
	}
	if (orig[yPixel / CELLSIZE][xPixel / CELLSIZE] == DEADEND) {
		wxMessageDialog* clickedDeadEnd = new wxMessageDialog(nullptr,
			wxT("You clicked on a dead end!"), wxT("Dead end!"), wxOK);
		clickedDeadEnd->ShowModal();
		delete clickedDeadEnd;
		return;
	}

	RecSolve(yPixel / CELLSIZE, xPixel / CELLSIZE);

	// force a full re-draw on the cMain wxFrame
	panel->Refresh();
	panel->Update();
}

void Maze::RecSolve(int row, int col)
{
	// You will need some base cases and some recursive cases here!
	// I leave it to you to think this through :)
	// base case: check if current cell is exit
	if (row < 0 || col < 0 || col > width - 1 || row > height - 1) return;

	if (solved[row][col] == EXIT) {
		free = true;
		wxMessageDialog* exitFound = new wxMessageDialog(nullptr,
			wxT("Exit found!"), wxT("Exit found!"), wxOK);
		exitFound->ShowModal();
		delete exitFound;
		return;
	}

	//check adjacent cells
	else {
		if (solved[row][col] != START)
			solved[row][col] = VISITED;

		panel->Update();
		panel->Refresh();


		if (row == 0)
		{
			if ((solved[row + 1][col] == EXIT) || (solved[row][col - 1] == EXIT) || (solved[row][col + 1] == EXIT))
			{
				if (solved[row + 1][col] == EXIT) { // going down
					row++;
				}
				else if (solved[row][col - 1] == EXIT) { // going left
					col--;
				}
				else if (solved[row][col + 1] == EXIT) { // going right
					col++;
				}
				if (row > -1 && col > -1)
					RecSolve(row, col);
			}
		}
		else if (row == height - 1)
		{
			if ((solved[row - 1][col] == EXIT) || (solved[row][col - 1] == EXIT) || (solved[row][col + 1] == EXIT))
			{
				if (solved[row - 1][col] == EXIT) { // going down
					row--;
				}
				else if (solved[row][col - 1] == EXIT) { // going left
					col--;
				}
				else if (solved[row][col + 1] == EXIT) { // going right
					col++;
				}
				if (row > -1 && col > -1)
					RecSolve(row, col);
			}
		}
		else {
			if (solved[row][col] != START)
				solved[row][col] = VISITED;

			if (row != 0) {
				if (solved[row - 1][col] == OPEN) {
					//if (row > -1 && col > -1)
					RecSolve(row - 1, col); // going up
				}
			}
			if (row != height - 1) {
				if (solved[row + 1][col] == OPEN) {
					//if (row > -1 && col > -1)
					RecSolve(row + 1, col); // going down
				}
			}
			if (true) {
				if (solved[row][col - 1] == OPEN) {
					//if (row > -1 && col > -1)
					RecSolve(row, col - 1); // going left
				}
			}
			if (true) {
				if (solved[row][col + 1] == OPEN) {
					//if (row > -1 && col > -1)
					RecSolve(row, col + 1); // going right
				}
			}
		}
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
	for (int i = 0; i < height; i++) // i=x
	{
		// Loop through columns
		for (int j = 0; j < width; j++) // j=y
		{
			// get the current maze cell
			char square = maze2Show[i][j];

			// the rest is up to you!
			// for every cell, you should draw a colored rectangle to 
			// wxFrame via the "dc" device context variable
			// I suggest looking into the DrawRectangle function			

			dc.SetPen(*wxBLACK_PEN);

			switch (square) {
			case OPEN:
				dc.SetBrush(*wxWHITE_BRUSH);
				break;
			case DEADEND:
				dc.SetBrush(*wxBLACK_BRUSH);
				break;
			case EXIT:
				dc.SetBrush(*wxGREEN_BRUSH);
				break;
			case START:
				dc.SetBrush(*wxRED_BRUSH);
				break;
			case VISITED:
				dc.SetBrush(*wxBLUE_BRUSH);

				break;
			default:
				valid = false;
			}
			dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
		}
	}

	panel->SetClientSize((width * CELLSIZE), (height * CELLSIZE));
}