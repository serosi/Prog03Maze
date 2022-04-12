#include "Maze.h"	// Maze class specification

Maze::~Maze()
{
	// Delete the dynamically allocated 2D arrays
	if (orig != nullptr)
	{
		for (int i = 0; i < height; i++)
			delete[] orig[i];
		delete[] orig;
	}
	if (solved != nullptr)
	{
		for (int i = 0; i < height; i++)
			delete[] solved[i];
		delete[] solved;
	}
}

// Maze constructor
Maze::Maze(wxFrame* parent, ifstream& ifs)
{
	// initially, the maze has not been solved, so set free to false
	free = false;
	// initially, the maze is invalid (we haven't read anything yet)
	valid = false;

	// set the panel to the passed in frame
	panel = parent; // a pointer to the parent GUI element
	orig = nullptr;
	solved = nullptr;
	maze2Show = orig;

	// Messages for invalid mazes
	wxMessageDialog* invalid = new wxMessageDialog(nullptr,
		wxT("The provided maze file contains invalid characters."), wxT("Maze Error"), wxOK);
	wxMessageDialog* tooBig = new wxMessageDialog(nullptr,
		wxT("The provided maze file is greater than 30x30 cells."), wxT("Size Error"), wxOK);

	ifs >> width >> height; // read in width and height
	if (width > MAXSIZE || height > MAXSIZE) {
		tooBig->ShowModal();
		return; // invalid
	}

	orig = new char* [height]; // dynamically create height arrays of size width
	for (int i = 0; i < height; i++) {
		orig[i] = new char[width];
	}
	solved = new char* [height];
	for (int i = 0; i < height; i++) {
		solved[i] = new char[width];
	}

	for (int i = 0; i < height; i++) {
		string mazeChars; // create string and read in characters line by line
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

	// delete message pointers
	delete tooBig;
	delete invalid;
	// make sure the maze is set to show the original, unmodified maze
	ShowOriginal();
}


// When the user clicks in the window and a maze object exists, this function
// will be called. It takes in user-supplied coordinates, xPixel and yPixel, and
// handles conditions like clicked on dead end, clicked on exit, and clicked on
// open cell. It displays appropriate messages or calls the RecSolve function to
// solve the maze. It will update and refresh the maze 
void Maze::Solve(int xPixel, int yPixel)
{
	// Define possible messages displayed in different function events
	wxMessageDialog* clickedExit = new wxMessageDialog(nullptr, wxT("You clicked on the exit!"), wxT("Exit found!"), wxOK);
	wxMessageDialog* clickedDeadEnd = new wxMessageDialog(nullptr,	wxT("You clicked on a dead end!"), wxT("Dead end!"), wxOK);
	wxMessageDialog* exitNotFound = new wxMessageDialog(nullptr, wxT("Could not find the exit."), wxT("Not Found"), wxOK);
	wxMessageDialog* exitFound = new wxMessageDialog(nullptr, wxT("Found the exit!"), wxT("Exit Found"), wxOK);

	// Reset the solved maze to be the orig maze and show the solved maze
	ResetSolution();
	ShowSolved();
	free = false; // Not free yet

	// Set clicked square to start
	solved[yPixel / CELLSIZE][xPixel / CELLSIZE] = START;

	// If exit was clicked on, display this message
	if (orig[yPixel / CELLSIZE][xPixel / CELLSIZE] == EXIT) {
		clickedExit->ShowModal();
		return;
	}

	// If clicked square = dead end, display this message
	if (orig[yPixel / CELLSIZE][xPixel / CELLSIZE] == DEADEND) {
		clickedDeadEnd->ShowModal();
		return;
	}

	// Recursively solve the maze
	RecSolve(yPixel / CELLSIZE, xPixel / CELLSIZE);

	if (free == true) // if free, display Found Exit message
		exitFound->ShowModal();
	else // You clicked on a square from which the exit could not be reached
		exitNotFound->ShowModal();
		
	
	// Delete pointers used for displaying messages
	delete clickedExit;
	delete clickedDeadEnd;
	delete exitFound;
	delete exitNotFound;
	// force a full re-draw on the cMain wxFrame
	panel->Refresh();
	panel->Update();
}


// Recursive function to find the exit of the maze using the clicked
// cell as a starting point. The base case is if the exit has been clicked.
// Otherwise it sets open squares to visited which are updated to be blue
// in the Show() function, and recursively loops through the maze until it
// finds the exit.
void Maze::RecSolve(int row, int col)
{
	if (solved[row][col] == EXIT)
		free = true;

	else {
		if (solved[row][col] != START)
			solved[row][col] = VISITED;

		if (row + 1 < height && ((solved[row + 1][col] == EXIT || solved[row + 1][col] == OPEN)))
			RecSolve(row + 1, col); // going down

		if (row - 1 >= 0 && ((solved[row - 1][col] == EXIT || solved[row - 1][col] == OPEN)))
			RecSolve(row - 1, col); // going up

		if (col + 1 < width && ((solved[row][col + 1] == EXIT || solved[row][col + 1] == OPEN)))
			RecSolve(row, col + 1); // going right

		if (col - 1 >= 0 && ((solved[row][col - 1] == EXIT || solved[row][col - 1] == OPEN)))
			RecSolve(row, col - 1); // going left
	}
}

// Copy over the solved array with the orig data
void Maze::ResetSolution()
{
	if ((orig != nullptr) && (solved != nullptr))
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				solved[i][j] = orig[i][j];
}

// Draws the maze to the screen using a grid of size CELLSIZE squares.
// Sets the color of the square to be different depending on what the
// current character of the maze data file is. Walls are black, open
// squares are white, the exit is green, the clicked cell is red, and
// areas where the recursion has gone are blue. Also sets the size of
// the panel to fit the provided maze file after it has been drawn.
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

			// set borders to black
			dc.SetPen(*wxBLACK_PEN);

			switch (square) { // set square color based on maze character
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
				valid = false; // invalid maze in any other case
			} // draw the rectangle based on cell size
			dc.DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE);
		}
	}

	// resize window
	panel->SetClientSize((width * CELLSIZE), (height * CELLSIZE));
}