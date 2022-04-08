#pragma once
#include "wx/wx.h"
#include "cMain.h"

class cApp : public wxApp
{
public:
	cApp();
	~cApp();

private:
	cMain* m_frame1 = nullptr;
	wxMenuBar* m_pMenuBar = nullptr;
	wxMenu* m_pFileMenu = nullptr;

public:
	virtual bool OnInit();
};

wxDECLARE_APP(cApp);

