#include "cApp.h"

wxIMPLEMENT_APP(cApp);

// Constructor and destructor
cApp::cApp() { }
cApp::~cApp() { }

// Creates a frame for the program to run in
bool cApp::OnInit() {
	m_frame1 = new cMain(); // create new frame
	m_frame1->Show(); // show the frame
	return true;
}