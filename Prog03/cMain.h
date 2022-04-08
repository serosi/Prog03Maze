#pragma once
#include "wx/wx.h"
#include "Maze.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

	// the event table is what maps user events to actual callback functions
	wxDECLARE_EVENT_TABLE();

	
	// private attributes of our class (and private functions)
private:
	// called whenever a redraw of the frame is needed - most GUI frameworks
	// have a similar kind of implementation
	void OnPaint(wxPaintEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnLeftClick(wxCommandEvent& event);
	// menu bar
	wxTextCtrl* m_pTextCtrl;
	wxMenuBar* m_pMenuBar;
	wxMenu* m_pFileMenu;
	wxMenu* m_pHelpMenu;
	wxString selectedFile;

	// attributes related to the maze
	Maze* maze = nullptr;

	// public attributes of our class (and public functions)
public:

};

