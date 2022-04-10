#include "cMain.h"

// This is the event table - we map any specific user events to their corresponding callback
// functions. You will need to do some searching on how to do this. I have given you a
// basic event table mapping that includes the EVT_PAINT event (calls OnPaint)
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
   EVT_PAINT(cMain::OnPaint)
   EVT_MENU(wxID_ABOUT, cMain::OnAbout)
   EVT_MENU(wxID_OPEN, cMain::OnOpen)
   EVT_MENU(wxID_EXIT, cMain::OnExit)
   EVT_LEFT_DOWN(cMain::GetPt)
wxEND_EVENT_TABLE()

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
cMain::~cMain()
{
}

// I'm giving you my implementation of the OnPaint event - this can be tricky to code even
// for experienced programmers. The OnPaint will be called anytime a re-draw of the frame
// is needed. You can also force a re-draw with: Refresh(); Update();
void cMain::OnPaint(wxPaintEvent& event)
{
   // need a devicec context to draw on
   wxPaintDC dc(this);

   // now, either draw the maze or print the "No Valid Maze Loaded" message
   if ((maze == nullptr) || !maze->IsValid())
   {
      wxString msg = wxT("No Valid Maze Loaded ...");
      wxPoint pt;
      pt.x = 0;
      pt.y = 0;
      dc.DrawText(msg, pt);
   }
   else
      if (maze->IsValid())
         maze->Show(dc);
}

void cMain::OnAbout(wxCommandEvent& event) {
   wxMessageDialog* dial = new wxMessageDialog(nullptr, 
      wxT("Program 3: Maze Solver by Maggie Nordquist"), wxT("About"), wxOK);
   dial->ShowModal();
   delete dial;
}

void cMain::OnOpen(wxCommandEvent& event) {
   // Create a new wxFileDialog dialog
   wxFileDialog* fileDialog = new wxFileDialog(this);

   if (fileDialog->ShowModal() == wxID_OK)
   {
      selectedFile = fileDialog->GetPath();
      ifstream ifs;
      ifs.open((const char*)selectedFile);
      maze = new Maze(this, ifs);
   }
   fileDialog->Destroy(); // close the file dialog frame
}

void cMain::OnExit(wxCommandEvent& event) {
   Destroy(); // destroy the current frame (exit program)
}

void cMain::GetPt(wxMouseEvent& event) {
   wxPoint mousePt = event.GetPosition();
   mousePt.x = mouseX;
   mousePt.y = mouseY;
}