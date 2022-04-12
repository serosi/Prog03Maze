#include "cMain.h"

// Event table: map any specific user events 
// to their corresponding callback functions
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
   EVT_PAINT(cMain::OnPaint)
   EVT_MENU(wxID_ABOUT, cMain::OnAbout)
   EVT_MENU(wxID_OPEN, cMain::OnOpen)
   EVT_MENU(wxID_EXIT, cMain::OnExit)
   EVT_LEFT_DOWN(cMain::OnLeftClick)
wxEND_EVENT_TABLE()


// cMain constructor
cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Program 3")
{
   m_pMenuBar = new wxMenuBar();
   // File Menu
   m_pFileMenu = new wxMenu();
   m_pFileMenu->Append(wxID_OPEN, _T("&Open Maze"));
   m_pFileMenu->AppendSeparator();
   m_pFileMenu->Append(wxID_EXIT, _T("&Quit"));
   m_pMenuBar->Append(m_pFileMenu, _T("&File"));
   // About menu
   m_pHelpMenu = new wxMenu();
   m_pHelpMenu->Append(wxID_ABOUT, _T("&About"));
   m_pMenuBar->Append(m_pHelpMenu, _T("&Help"));

   SetMenuBar(m_pMenuBar);
}

// destructor - nothing to do
cMain::~cMain() { }

// I'm giving you my implementation of the OnPaint event - this can be tricky to code even
// for experienced programmers. The OnPaint will be called anytime a re-draw of the frame
// is needed. You can also force a re-draw with: Refresh(); Update();
void cMain::OnPaint(wxPaintEvent& event)
{
   // need a device context to draw on
   wxPaintDC dc(this);

   // now, either draw the maze or print the "No Valid Maze Loaded" message
   if ((maze == nullptr) || !maze->IsValid())
   {
      // if the user has not selected a valid maze or has not selected any maze,
      // display this message
      wxString msg = wxT("There is no valid maze loaded.");
      wxPoint pt;
      pt.x = 5;
      pt.y = 3;
      dc.DrawText(msg, pt);
   }
   else
      if (maze->IsValid()) // If valid maze selected, show it on the screen
         maze->Show(dc);
}


// Displays an About message when the user selects the About option
// in the menu.
void cMain::OnAbout(wxCommandEvent& event) {
   wxMessageDialog* dial = new wxMessageDialog(nullptr, 
      wxT("Program 3: Maze Solver by Maggie Nordquist"), wxT("About"), wxOK);
   dial->ShowModal();
   delete dial;
}

// Function to open a file and assign the file path to an ifstream
// object. Spawns a file dialog window and allows the user to select
// a file, then create a new maze object using the file and finally
// removes the file dialog.
void cMain::OnOpen(wxCommandEvent& event) {
   // Create a new wxFileDialog dialog
   wxFileDialog* fileDialog = new wxFileDialog(this);

   if (fileDialog->ShowModal() == wxID_OK) {
      selectedFile = fileDialog->GetPath();
      ifstream ifs; // create new ifstream obj
      ifs.open((const char*)selectedFile);
      maze = new Maze(this, ifs); // create new maze obj
   }
   fileDialog->Destroy(); // close the file dialog frame
   Refresh();
}

// Called upon exit, destroys current frame
void cMain::OnExit(wxCommandEvent& event) {
   Destroy(); // destroy the current frame (exit program)
}

// When the user clicks the main window, gets the coordinates of
// the mouse pointer on click and if the maze object exists, it
// calls Solve() using the clicked coordinates as parameters.
void cMain::OnLeftClick(wxMouseEvent& event) {
   wxPoint mousePt = event.GetPosition();
   if (maze != nullptr)
      maze->Solve(mousePt.x, mousePt.y);
}