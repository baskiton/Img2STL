#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Img2STLMainFrame.h"
#include "app_icon.xpm"


class MyApp : public wxApp {
public:
    bool OnInit() override;
};

bool MyApp::OnInit() {
    auto *frame = new Img2STLMainFrame("Img2STL");
    wxInitAllImageHandlers();
    frame->SetIcon(wxICON(app_icon));
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}

wxIMPLEMENT_APP(MyApp);
