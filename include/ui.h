///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/radiobox.h>
#include <wx/gauge.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/scrolwin.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* menubar;
		wxMenu* menu_file;
		wxMenu* menu_help;
		wxSplitterWindow* main_container;
		wxPanel* prop_panel;
		wxButton* prop_image_open;
		wxCheckBox* prop_image_autocrop;
		wxStaticText* prop_den_lable;
		wxSpinCtrlDouble* prop_den_val;
		wxStaticText* prop_max_height_lable;
		wxSpinCtrlDouble* prop_max_height_val;
		wxStaticText* prop_min_height_lable;
		wxSpinCtrlDouble* prop_min_height_val;
		wxStaticText* prop_mask_mode_lable;
		wxChoice* prop_mask_mode_val;
		wxStaticText* prop_transp_col_lable;
		wxButton* prop_transp_col_val;
		wxStaticText* prop_height_mode_lable;
		wxChoice* prop_height_mode_val;
		wxStaticText* prop_exit_label;
		wxButton* prop_exit_btn;
		wxRadioBox* prop_exit_format;
		wxButton* execute_button;
		wxGauge* progress_bar;
		wxStaticText* progress_label;
		wxScrolledWindow* imagepanel;
		wxSplitterWindow* image_panel_container;
		wxScrolledWindow* image_container;
		wxAuiNotebook* note;
		wxScrolledWindow* image_info_panel;
		wxPropertyGrid* palette_grid;
		wxPGProperty* palette_header;
		wxStatusBar* statusBar;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void main_containerOnSplitterSashPosChanging( wxSplitterEvent& event ) { event.Skip(); }
		virtual void prop_image_openOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void prop_image_autocropOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void prop_mask_mode_valOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void prop_transp_col_valOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void prop_exit_btnOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void execute_buttonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void image_panel_containerOnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void image_panel_containerOnSplitterSashPosChanged( wxSplitterEvent& event ) { event.Skip(); }
		virtual void noteOnAuiNotebookPageChanged( wxAuiNotebookEvent& event ) { event.Skip(); }
		virtual void noteOnAuiNotebookPageClose( wxAuiNotebookEvent& event ) { event.Skip(); }


	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );

		~MainFrame();

		void main_containerOnIdle( wxIdleEvent& )
		{
			main_container->SetSashPosition( 280 );
			main_container->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::main_containerOnIdle ), NULL, this );
		}

};

