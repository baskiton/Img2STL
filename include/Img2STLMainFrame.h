#ifndef __Img2STLMainFrame__
#define __Img2STLMainFrame__

#include <vector>

#include "wx/image.h"
#include "wx/filename.h"

#include "ui.h"
#include "Executor.h"


class Img2STLMainFrame : public MainFrame
{
protected:
    // Handlers for MainFrame events.
    void OnClose( wxCloseEvent& event ) wxOVERRIDE;
    void OnExit( wxCommandEvent& event ) wxOVERRIDE;
    void OnAbout( wxCommandEvent& event ) wxOVERRIDE;
    void main_containerOnSplitterSashPosChanging( wxSplitterEvent& event ) wxOVERRIDE;
    void prop_image_openOnButtonClick( wxCommandEvent& event ) wxOVERRIDE;
    void prop_image_autocropOnCheckBox( wxCommandEvent& event ) wxOVERRIDE;
    void prop_mask_mode_valOnChoice( wxCommandEvent& event ) wxOVERRIDE;
    void prop_transp_col_valOnButtonClick( wxCommandEvent& event ) wxOVERRIDE;
    void prop_exit_btnOnButtonClick( wxCommandEvent& event ) wxOVERRIDE;
    void execute_buttonOnButtonClick( wxCommandEvent& event ) wxOVERRIDE;
    void image_panel_containerOnSize( wxSizeEvent& event ) wxOVERRIDE;
    void image_panel_containerOnSplitterSashPosChanged( wxSplitterEvent& event ) wxOVERRIDE;

    void noteOnAuiNotebookPageChanged( wxAuiNotebookEvent& event ) wxOVERRIDE;
    void noteOnAuiNotebookPageClose( wxAuiNotebookEvent& event ) wxOVERRIDE;

    void OnThreadUpdate(wxThreadEvent &event);

public:
    explicit Img2STLMainFrame(const wxString &title);

private:
    wxThread *m_executor;
    wxCriticalSection m_executor_CS;

    friend class Executor;

    wxString files_directory;
    wxString exit_files_dir;
    wxArrayString files_list;
    wxArrayString exit_files;
    wxPathList fl;
    std::vector<wxFileName> img_files;
    std::vector<wxFileName> stl_files;

    void add_image_tabs();
    void change_image_scale();
    void set_prop_exit_label();
    void fill_palette(wxImage *img);
};

#endif // __Img2STLMainFrame__
