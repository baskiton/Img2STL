///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ui.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 700,530 ), wxDefaultSize );
	this->SetFont( wxFont( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Noto Sans") ) );

	m_menu_bar = new wxMenuBar( 0 );
	m_menu_bar->SetFont( wxFont( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Noto Sans") ) );

	m_menu_file = new wxMenu();
	m_menu_file->AppendSeparator();

	wxMenuItem* m_menu_file_exit;
	m_menu_file_exit = new wxMenuItem( m_menu_file, wxID_EXIT, wxString( wxT("&Quit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menu_file_exit );

	m_menu_bar->Append( m_menu_file, wxT("File") );

	m_menu_help = new wxMenu();
	m_menu_help->AppendSeparator();

	wxMenuItem* m_menu_help_about;
	m_menu_help_about = new wxMenuItem( m_menu_help, wxID_ABOUT, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menu_help_about );

	m_menu_bar->Append( m_menu_help, wxT("Help") );

	this->SetMenuBar( m_menu_bar );

	wxBoxSizer* m_main_sizer;
	m_main_sizer = new wxBoxSizer( wxVERTICAL );

	m_main_container = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE|wxSP_NOSASH );
	m_main_container->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::m_main_containerOnIdle ), NULL, this );

	m_prop_panel = new wxPanel( m_main_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_prop_panel->SetMinSize( wxSize( 200,-1 ) );

	wxBoxSizer* m_prop_sizer;
	m_prop_sizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* m_prop_image_open_sizer;
	m_prop_image_open_sizer = new wxStaticBoxSizer( new wxStaticBox( m_prop_panel, wxID_ANY, wxT("Open Image") ), wxHORIZONTAL );

	m_prop_image_open = new wxButton( m_prop_image_open_sizer->GetStaticBox(), wxID_OPEN, wxT("Open Image(s)"), wxDefaultPosition, wxDefaultSize, 0 );

	m_prop_image_open->SetDefault();
	m_prop_image_open_sizer->Add( m_prop_image_open, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_prop_image_autocrop = new wxCheckBox( m_prop_image_open_sizer->GetStaticBox(), wxID_ANY, wxT("Auto Crop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_image_autocrop->SetValue(true);
	m_prop_image_open_sizer->Add( m_prop_image_autocrop, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_prop_sizer->Add( m_prop_image_open_sizer, 0, wxRIGHT|wxLEFT|wxEXPAND, 5 );

	wxStaticBoxSizer* m_prop_prop_sizer;
	m_prop_prop_sizer = new wxStaticBoxSizer( new wxStaticBox( m_prop_panel, wxID_ANY, wxT("Property") ), wxVERTICAL );

	wxFlexGridSizer* m_prop_prop_grid;
	m_prop_prop_grid = new wxFlexGridSizer( 0, 2, 3, 3 );
	m_prop_prop_grid->SetFlexibleDirection( wxBOTH );
	m_prop_prop_grid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_prop_den_lable = new wxStaticText( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Density (mm/pix)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_den_lable->Wrap( -1 );
	m_prop_prop_grid->Add( m_prop_den_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_prop_den_val = new wxSpinCtrlDouble( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, 0, 1000, 2, 0.001 );
	m_prop_den_val->SetDigits( 3 );
	m_prop_prop_grid->Add( m_prop_den_val, 0, wxEXPAND|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_prop_max_height_lable = new wxStaticText( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Max Height (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_max_height_lable->Wrap( -1 );
	m_prop_prop_grid->Add( m_prop_max_height_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_prop_max_height_val = new wxSpinCtrlDouble( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, 0, 1000, 4, 0.001 );
	m_prop_max_height_val->SetDigits( 3 );
	m_prop_prop_grid->Add( m_prop_max_height_val, 0, wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_prop_min_height_lable = new wxStaticText( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Min Height (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_min_height_lable->Wrap( -1 );
	m_prop_prop_grid->Add( m_prop_min_height_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_prop_min_height_val = new wxSpinCtrlDouble( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, 0, 1000, 1, 0.001 );
	m_prop_min_height_val->SetDigits( 3 );
	m_prop_prop_grid->Add( m_prop_min_height_val, 0, wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_prop_mask_mode_lable = new wxStaticText( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Mask Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_mask_mode_lable->Wrap( -1 );
	m_prop_prop_grid->Add( m_prop_mask_mode_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	wxString m_prop_mask_mode_valChoices[] = { wxT("Transparency"), wxT("Color") };
	int m_prop_mask_mode_valNChoices = sizeof( m_prop_mask_mode_valChoices ) / sizeof( wxString );
	m_prop_mask_mode_val = new wxChoice( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), m_prop_mask_mode_valNChoices, m_prop_mask_mode_valChoices, 0 );
	m_prop_mask_mode_val->SetSelection( 0 );
	m_prop_prop_grid->Add( m_prop_mask_mode_val, 0, wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_prop_transp_col_lable = new wxStaticText( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Transparency color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_transp_col_lable->Wrap( -1 );
	m_prop_transp_col_lable->Enable( false );

	m_prop_prop_grid->Add( m_prop_transp_col_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_prop_transp_col_val = new wxButton( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("#FF00FF"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
	m_prop_transp_col_val->SetBackgroundColour( wxColour( 255, 0, 255 ) );
	m_prop_transp_col_val->Enable( false );

	m_prop_prop_grid->Add( m_prop_transp_col_val, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_prop_height_mode_lable = new wxStaticText( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Height Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_height_mode_lable->Wrap( -1 );
	m_prop_prop_grid->Add( m_prop_height_mode_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	wxString m_prop_height_mode_valChoices[] = { wxT("Black"), wxT("White") };
	int m_prop_height_mode_valNChoices = sizeof( m_prop_height_mode_valChoices ) / sizeof( wxString );
	m_prop_height_mode_val = new wxChoice( m_prop_prop_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), m_prop_height_mode_valNChoices, m_prop_height_mode_valChoices, 0 );
	m_prop_height_mode_val->SetSelection( 1 );
	m_prop_prop_grid->Add( m_prop_height_mode_val, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	m_prop_prop_sizer->Add( m_prop_prop_grid, 0, 0, 5 );


	m_prop_sizer->Add( m_prop_prop_sizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* m_prop_exit_sizer;
	m_prop_exit_sizer = new wxStaticBoxSizer( new wxStaticBox( m_prop_panel, wxID_ANY, wxT("Exit File") ), wxVERTICAL );

	wxBoxSizer* m_prop_exit_grid;
	m_prop_exit_grid = new wxBoxSizer( wxHORIZONTAL );

	m_prop_exit_label = new wxStaticText( m_prop_exit_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT|wxST_ELLIPSIZE_MIDDLE|wxST_NO_AUTORESIZE );
	m_prop_exit_label->Wrap( -1 );
	m_prop_exit_grid->Add( m_prop_exit_label, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_prop_exit_btn = new wxButton( m_prop_exit_sizer->GetStaticBox(), wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	m_prop_exit_btn->Enable( false );

	m_prop_exit_grid->Add( m_prop_exit_btn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );


	m_prop_exit_sizer->Add( m_prop_exit_grid, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxString m_prop_exit_formatChoices[] = { wxT("ASCII"), wxT("BIN") };
	int m_prop_exit_formatNChoices = sizeof( m_prop_exit_formatChoices ) / sizeof( wxString );
	m_prop_exit_format = new wxRadioBox( m_prop_exit_sizer->GetStaticBox(), wxID_ANY, wxT("STL format"), wxDefaultPosition, wxDefaultSize, m_prop_exit_formatNChoices, m_prop_exit_formatChoices, 1, wxRA_SPECIFY_ROWS );
	m_prop_exit_format->SetSelection( 1 );
	m_prop_exit_sizer->Add( m_prop_exit_format, 0, wxBOTTOM|wxLEFT, 5 );


	m_prop_sizer->Add( m_prop_exit_sizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* m_prop_execute_sizer;
	m_prop_execute_sizer = new wxStaticBoxSizer( new wxStaticBox( m_prop_panel, wxID_ANY, wxT("Execute") ), wxVERTICAL );

	m_execute_button = new wxButton( m_prop_execute_sizer->GetStaticBox(), wxID_ANY, wxT("Execute"), wxDefaultPosition, wxDefaultSize, 0 );

	m_execute_button->SetDefault();
	m_execute_button->Enable( false );

	m_prop_execute_sizer->Add( m_execute_button, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_progress_bar = new wxGauge( m_prop_execute_sizer->GetStaticBox(), wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH );
	m_progress_bar->SetValue( 0 );
	m_prop_execute_sizer->Add( m_progress_bar, 0, wxALL|wxEXPAND, 5 );

	m_progress_label = new wxStaticText( m_prop_execute_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_progress_label->Wrap( -1 );
	m_prop_execute_sizer->Add( m_progress_label, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 5 );


	m_prop_sizer->Add( m_prop_execute_sizer, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	m_prop_panel->SetSizer( m_prop_sizer );
	m_prop_panel->Layout();
	m_prop_sizer->Fit( m_prop_panel );
	m_imagepanel = new wxScrolledWindow( m_main_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_imagepanel->SetScrollRate( 5, 5 );
	wxBoxSizer* m_image_panel_sizer;
	m_image_panel_sizer = new wxBoxSizer( wxVERTICAL );

	m_image_panel_container = new wxSplitterWindow( m_imagepanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE|wxSP_NOSASH );
	m_image_container = new wxScrolledWindow( m_image_panel_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxHSCROLL|wxVSCROLL );
	m_image_container->SetScrollRate( 5, 5 );
	wxBoxSizer* m_image_sizer;
	m_image_sizer = new wxBoxSizer( wxVERTICAL );

	m_note = new wxAuiNotebook( m_image_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ALL_TABS|wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );

	m_image_sizer->Add( m_note, 1, wxEXPAND, 5 );


	m_image_container->SetSizer( m_image_sizer );
	m_image_container->Layout();
	m_image_sizer->Fit( m_image_container );
	m_image_info_panel = new wxScrolledWindow( m_image_panel_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_image_info_panel->SetScrollRate( 5, 5 );
	m_image_info_panel->SetMinSize( wxSize( 200,-1 ) );

	wxBoxSizer* m_image_info_panel_sizer;
	m_image_info_panel_sizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* m_palette_sizer;
	m_palette_sizer = new wxStaticBoxSizer( new wxStaticBox( m_image_info_panel, wxID_ANY, wxT("Palette") ), wxVERTICAL );

	m_palette_grid = new wxPropertyGrid(m_palette_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_HIDE_MARGIN|wxPG_LIMITED_EDITING);
	m_palette_grid->SetMaxSize( wxSize( 200,-1 ) );

	m_palette_header = m_palette_grid->Append( new wxPropertyCategory( wxT("amout | color"), wxT("amout | color") ) );
	m_palette_sizer->Add( m_palette_grid, 1, wxEXPAND, 5 );


	m_image_info_panel_sizer->Add( m_palette_sizer, 1, wxEXPAND, 5 );


	m_image_info_panel->SetSizer( m_image_info_panel_sizer );
	m_image_info_panel->Layout();
	m_image_info_panel_sizer->Fit( m_image_info_panel );
	m_image_panel_container->SplitVertically( m_image_container, m_image_info_panel, -1 );
	m_image_panel_sizer->Add( m_image_panel_container, 1, wxEXPAND, 5 );


	m_imagepanel->SetSizer( m_image_panel_sizer );
	m_imagepanel->Layout();
	m_image_panel_sizer->Fit( m_imagepanel );
	m_main_container->SplitVertically( m_prop_panel, m_imagepanel, 280 );
	m_main_sizer->Add( m_main_container, 1, wxEXPAND, 5 );


	this->SetSizer( m_main_sizer );
	this->Layout();
	m_status_bar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	m_menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExit ), this, m_menu_file_exit->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAbout ), this, m_menu_help_about->GetId());
	m_main_container->Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING, wxSplitterEventHandler( MainFrame::main_containerOnSplitterSashPosChanging ), NULL, this );
	m_prop_image_open->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_image_openOnButtonClick ), NULL, this );
	m_prop_image_autocrop->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::prop_image_autocropOnCheckBox ), NULL, this );
	m_prop_mask_mode_val->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::prop_mask_mode_valOnChoice ), NULL, this );
	m_prop_transp_col_val->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_transp_col_valOnButtonClick ), NULL, this );
	m_prop_exit_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_exit_btnOnButtonClick ), NULL, this );
	m_execute_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::execute_buttonOnButtonClick ), NULL, this );
	m_image_panel_container->Connect( wxEVT_SIZE, wxSizeEventHandler( MainFrame::image_panel_containerOnSize ), NULL, this );
	m_image_panel_container->Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( MainFrame::image_panel_containerOnSplitterSashPosChanged ), NULL, this );
	m_note->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageChanged ), NULL, this );
	m_note->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageClose ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	m_main_container->Disconnect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING, wxSplitterEventHandler( MainFrame::main_containerOnSplitterSashPosChanging ), NULL, this );
	m_prop_image_open->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_image_openOnButtonClick ), NULL, this );
	m_prop_image_autocrop->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::prop_image_autocropOnCheckBox ), NULL, this );
	m_prop_mask_mode_val->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::prop_mask_mode_valOnChoice ), NULL, this );
	m_prop_transp_col_val->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_transp_col_valOnButtonClick ), NULL, this );
	m_prop_exit_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_exit_btnOnButtonClick ), NULL, this );
	m_execute_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::execute_buttonOnButtonClick ), NULL, this );
	m_image_panel_container->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainFrame::image_panel_containerOnSize ), NULL, this );
	m_image_panel_container->Disconnect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( MainFrame::image_panel_containerOnSplitterSashPosChanged ), NULL, this );
	m_note->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageChanged ), NULL, this );
	m_note->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageClose ), NULL, this );

}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxSize( -1,-1 ) );

	m_sizer_main = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerInMain;
	bSizerInMain = new wxBoxSizer( wxHORIZONTAL );

	m_app_icon = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 32,32 ), 0 );
	bSizerInMain->Add( m_app_icon, 0, wxALL, 5 );

	wxBoxSizer* bSizerDesc;
	bSizerDesc = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerName;
	bSizerName = new wxBoxSizer( wxHORIZONTAL );


	bSizerName->Add( 0, 0, 1, wxEXPAND, 5 );

	m_name_version = new wxStaticText( this, wxID_ANY, wxT("%name% %version%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_name_version->Wrap( -1 );
	m_name_version->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizerName->Add( m_name_version, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerName->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerDesc->Add( bSizerName, 1, wxEXPAND, 5 );

	m_description = new wxStaticText( this, wxID_ANY, wxT("%description%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_description->Wrap( -1 );
	bSizerDesc->Add( m_description, 0, wxALL|wxEXPAND, 5 );

	m_using_str = new wxStaticText( this, wxID_ANY, wxT("Using %wx%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_using_str->Wrap( -1 );
	bSizerDesc->Add( m_using_str, 0, wxEXPAND|wxALL, 5 );

	m_url_sizer = new wxBoxSizer( wxVERTICAL );


	bSizerDesc->Add( m_url_sizer, 1, wxEXPAND, 5 );


	bSizerInMain->Add( bSizerDesc, 1, wxEXPAND, 5 );


	m_sizer_main->Add( bSizerInMain, 0, wxEXPAND, 5 );

	m_license_pane = new wxCollapsiblePane( this, wxID_ANY, wxT("License"), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
	m_license_pane->Collapse( true );

	m_sizer_license = new wxBoxSizer( wxVERTICAL );


	m_license_pane->GetPane()->SetSizer( m_sizer_license );
	m_license_pane->GetPane()->Layout();
	m_sizer_license->Fit( m_license_pane->GetPane() );
	m_sizer_main->Add( m_license_pane, 1, wxEXPAND|wxALL, 5 );

	wxBoxSizer* bSizerLow;
	bSizerLow = new wxBoxSizer( wxHORIZONTAL );

	m_copyright = new wxHyperlinkCtrl( this, wxID_ANY, wxT("%label%"), wxT("%url%"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizerLow->Add( m_copyright, 1, wxALL|wxALIGN_BOTTOM, 5 );

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizer->Realize();

	bSizerLow->Add( m_sdbSizer, 0, wxEXPAND|wxALL, 5 );


	m_sizer_main->Add( bSizerLow, 0, wxEXPAND, 5 );


	this->SetSizer( m_sizer_main );
	this->Layout();
	m_sizer_main->Fit( this );

	this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
}
