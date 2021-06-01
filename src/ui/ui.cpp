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

	menubar = new wxMenuBar( 0 );
	menubar->SetFont( wxFont( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Noto Sans") ) );

	menu_file = new wxMenu();
	menu_file->AppendSeparator();

	wxMenuItem* menu_file_exit;
	menu_file_exit = new wxMenuItem( menu_file, wxID_EXIT, wxString( wxT("&Quit") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( menu_file_exit );

	menubar->Append( menu_file, wxT("File") );

	menu_help = new wxMenu();
	menu_help->AppendSeparator();

	wxMenuItem* menu_help_about;
	menu_help_about = new wxMenuItem( menu_help, wxID_ABOUT, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	menu_help->Append( menu_help_about );

	menubar->Append( menu_help, wxT("Help") );

	this->SetMenuBar( menubar );

	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );

	main_container = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE|wxSP_NOSASH );
	main_container->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::main_containerOnIdle ), NULL, this );

	prop_panel = new wxPanel( main_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	prop_panel->SetMinSize( wxSize( 200,-1 ) );

	wxBoxSizer* prop_sizer;
	prop_sizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* prop_image_open_sizer;
	prop_image_open_sizer = new wxStaticBoxSizer( new wxStaticBox( prop_panel, wxID_ANY, wxT("Open Image") ), wxHORIZONTAL );

	prop_image_open = new wxButton( prop_image_open_sizer->GetStaticBox(), wxID_OPEN, wxT("Open Image(s)"), wxDefaultPosition, wxDefaultSize, 0 );

	prop_image_open->SetDefault();
	prop_image_open_sizer->Add( prop_image_open, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	prop_image_autocrop = new wxCheckBox( prop_image_open_sizer->GetStaticBox(), wxID_ANY, wxT("Auto Crop"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_image_autocrop->SetValue(true);
	prop_image_open_sizer->Add( prop_image_autocrop, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	prop_sizer->Add( prop_image_open_sizer, 0, wxRIGHT|wxLEFT|wxEXPAND, 5 );

	wxStaticBoxSizer* prop_prop_sizer;
	prop_prop_sizer = new wxStaticBoxSizer( new wxStaticBox( prop_panel, wxID_ANY, wxT("Property") ), wxVERTICAL );

	wxFlexGridSizer* prop_prop_grid;
	prop_prop_grid = new wxFlexGridSizer( 0, 2, 3, 3 );
	prop_prop_grid->SetFlexibleDirection( wxBOTH );
	prop_prop_grid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	prop_den_lable = new wxStaticText( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Density (mm/pix)"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_den_lable->Wrap( -1 );
	prop_prop_grid->Add( prop_den_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	prop_den_val = new wxSpinCtrlDouble( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, 0, 1000, 2, 0.001 );
	prop_den_val->SetDigits( 3 );
	prop_prop_grid->Add( prop_den_val, 0, wxEXPAND|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	prop_max_height_lable = new wxStaticText( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Max Height (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_max_height_lable->Wrap( -1 );
	prop_prop_grid->Add( prop_max_height_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	prop_max_height_val = new wxSpinCtrlDouble( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, 0, 1000, 4, 0.001 );
	prop_max_height_val->SetDigits( 3 );
	prop_prop_grid->Add( prop_max_height_val, 0, wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	prop_min_height_lable = new wxStaticText( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Min Height (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_min_height_lable->Wrap( -1 );
	prop_prop_grid->Add( prop_min_height_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	prop_min_height_val = new wxSpinCtrlDouble( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, 0, 1000, 1, 0.001 );
	prop_min_height_val->SetDigits( 3 );
	prop_prop_grid->Add( prop_min_height_val, 0, wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	prop_mask_mode_lable = new wxStaticText( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Mask Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_mask_mode_lable->Wrap( -1 );
	prop_prop_grid->Add( prop_mask_mode_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	wxString prop_mask_mode_valChoices[] = { wxT("Transparency"), wxT("Color") };
	int prop_mask_mode_valNChoices = sizeof( prop_mask_mode_valChoices ) / sizeof( wxString );
	prop_mask_mode_val = new wxChoice( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), prop_mask_mode_valNChoices, prop_mask_mode_valChoices, 0 );
	prop_mask_mode_val->SetSelection( 0 );
	prop_prop_grid->Add( prop_mask_mode_val, 0, wxRIGHT|wxLEFT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	prop_transp_col_lable = new wxStaticText( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Transparency color"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_transp_col_lable->Wrap( -1 );
	prop_transp_col_lable->Enable( false );

	prop_prop_grid->Add( prop_transp_col_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	prop_transp_col_val = new wxButton( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("#FF00FF"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
	prop_transp_col_val->SetBackgroundColour( wxColour( 255, 0, 255 ) );
	prop_transp_col_val->Enable( false );

	prop_prop_grid->Add( prop_transp_col_val, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	prop_height_mode_lable = new wxStaticText( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxT("Height Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_height_mode_lable->Wrap( -1 );
	prop_prop_grid->Add( prop_height_mode_lable, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	wxString prop_height_mode_valChoices[] = { wxT("Black"), wxT("White") };
	int prop_height_mode_valNChoices = sizeof( prop_height_mode_valChoices ) / sizeof( wxString );
	prop_height_mode_val = new wxChoice( prop_prop_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), prop_height_mode_valNChoices, prop_height_mode_valChoices, 0 );
	prop_height_mode_val->SetSelection( 1 );
	prop_prop_grid->Add( prop_height_mode_val, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	prop_prop_sizer->Add( prop_prop_grid, 0, 0, 5 );


	prop_sizer->Add( prop_prop_sizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* prop_exit_sizer;
	prop_exit_sizer = new wxStaticBoxSizer( new wxStaticBox( prop_panel, wxID_ANY, wxT("Exit File") ), wxVERTICAL );

	wxBoxSizer* prop_exit_grid;
	prop_exit_grid = new wxBoxSizer( wxHORIZONTAL );

	prop_exit_label = new wxStaticText( prop_exit_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT|wxST_ELLIPSIZE_MIDDLE|wxST_NO_AUTORESIZE );
	prop_exit_label->Wrap( -1 );
	prop_exit_grid->Add( prop_exit_label, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	prop_exit_btn = new wxButton( prop_exit_sizer->GetStaticBox(), wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	prop_exit_btn->Enable( false );

	prop_exit_grid->Add( prop_exit_btn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );


	prop_exit_sizer->Add( prop_exit_grid, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	wxString prop_exit_formatChoices[] = { wxT("ASCII"), wxT("BIN") };
	int prop_exit_formatNChoices = sizeof( prop_exit_formatChoices ) / sizeof( wxString );
	prop_exit_format = new wxRadioBox( prop_exit_sizer->GetStaticBox(), wxID_ANY, wxT("STL format"), wxDefaultPosition, wxDefaultSize, prop_exit_formatNChoices, prop_exit_formatChoices, 1, wxRA_SPECIFY_ROWS );
	prop_exit_format->SetSelection( 1 );
	prop_exit_sizer->Add( prop_exit_format, 0, wxBOTTOM|wxLEFT, 5 );


	prop_sizer->Add( prop_exit_sizer, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* prop_execute_sizer;
	prop_execute_sizer = new wxStaticBoxSizer( new wxStaticBox( prop_panel, wxID_ANY, wxT("Execute") ), wxVERTICAL );

	execute_button = new wxButton( prop_execute_sizer->GetStaticBox(), wxID_ANY, wxT("Execute"), wxDefaultPosition, wxDefaultSize, 0 );

	execute_button->SetDefault();
	execute_button->Enable( false );

	prop_execute_sizer->Add( execute_button, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	progress_bar = new wxGauge( prop_execute_sizer->GetStaticBox(), wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH );
	progress_bar->SetValue( 0 );
	prop_execute_sizer->Add( progress_bar, 0, wxALL|wxEXPAND, 5 );

	progress_label = new wxStaticText( prop_execute_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	progress_label->Wrap( -1 );
	prop_execute_sizer->Add( progress_label, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 5 );


	prop_sizer->Add( prop_execute_sizer, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	prop_panel->SetSizer( prop_sizer );
	prop_panel->Layout();
	prop_sizer->Fit( prop_panel );
	imagepanel = new wxScrolledWindow( main_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	imagepanel->SetScrollRate( 5, 5 );
	wxBoxSizer* image_panel_sizer;
	image_panel_sizer = new wxBoxSizer( wxVERTICAL );

	image_panel_container = new wxSplitterWindow( imagepanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE|wxSP_NOSASH );
	image_container = new wxScrolledWindow( image_panel_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxHSCROLL|wxVSCROLL );
	image_container->SetScrollRate( 5, 5 );
	wxBoxSizer* image_sizer;
	image_sizer = new wxBoxSizer( wxVERTICAL );

	note = new wxAuiNotebook( image_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ALL_TABS|wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );

	image_sizer->Add( note, 1, wxEXPAND, 5 );


	image_container->SetSizer( image_sizer );
	image_container->Layout();
	image_sizer->Fit( image_container );
	image_info_panel = new wxScrolledWindow( image_panel_container, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	image_info_panel->SetScrollRate( 5, 5 );
	image_info_panel->SetMinSize( wxSize( 200,-1 ) );

	wxBoxSizer* image_info_panel_sizer;
	image_info_panel_sizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* palette_sizer;
	palette_sizer = new wxStaticBoxSizer( new wxStaticBox( image_info_panel, wxID_ANY, wxT("Palette") ), wxVERTICAL );

	palette_grid = new wxPropertyGrid(palette_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_HIDE_MARGIN|wxPG_LIMITED_EDITING);
	palette_grid->SetMaxSize( wxSize( 200,-1 ) );

	palette_header = palette_grid->Append( new wxPropertyCategory( wxT("amout | color"), wxT("amout | color") ) );
	palette_sizer->Add( palette_grid, 1, wxEXPAND, 5 );


	image_info_panel_sizer->Add( palette_sizer, 1, wxEXPAND, 5 );


	image_info_panel->SetSizer( image_info_panel_sizer );
	image_info_panel->Layout();
	image_info_panel_sizer->Fit( image_info_panel );
	image_panel_container->SplitVertically( image_container, image_info_panel, -1 );
	image_panel_sizer->Add( image_panel_container, 1, wxEXPAND, 5 );


	imagepanel->SetSizer( image_panel_sizer );
	imagepanel->Layout();
	image_panel_sizer->Fit( imagepanel );
	main_container->SplitVertically( prop_panel, imagepanel, 280 );
	main_sizer->Add( main_container, 1, wxEXPAND, 5 );


	this->SetSizer( main_sizer );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExit ), this, menu_file_exit->GetId());
	menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAbout ), this, menu_help_about->GetId());
	main_container->Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING, wxSplitterEventHandler( MainFrame::main_containerOnSplitterSashPosChanging ), NULL, this );
	prop_image_open->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_image_openOnButtonClick ), NULL, this );
	prop_image_autocrop->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::prop_image_autocropOnCheckBox ), NULL, this );
	prop_mask_mode_val->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::prop_mask_mode_valOnChoice ), NULL, this );
	prop_transp_col_val->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_transp_col_valOnButtonClick ), NULL, this );
	prop_exit_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_exit_btnOnButtonClick ), NULL, this );
	execute_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::execute_buttonOnButtonClick ), NULL, this );
	image_panel_container->Connect( wxEVT_SIZE, wxSizeEventHandler( MainFrame::image_panel_containerOnSize ), NULL, this );
	image_panel_container->Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( MainFrame::image_panel_containerOnSplitterSashPosChanged ), NULL, this );
	note->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageChanged ), NULL, this );
	note->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageClose ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	main_container->Disconnect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING, wxSplitterEventHandler( MainFrame::main_containerOnSplitterSashPosChanging ), NULL, this );
	prop_image_open->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_image_openOnButtonClick ), NULL, this );
	prop_image_autocrop->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::prop_image_autocropOnCheckBox ), NULL, this );
	prop_mask_mode_val->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::prop_mask_mode_valOnChoice ), NULL, this );
	prop_transp_col_val->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_transp_col_valOnButtonClick ), NULL, this );
	prop_exit_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::prop_exit_btnOnButtonClick ), NULL, this );
	execute_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::execute_buttonOnButtonClick ), NULL, this );
	image_panel_container->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainFrame::image_panel_containerOnSize ), NULL, this );
	image_panel_container->Disconnect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( MainFrame::image_panel_containerOnSplitterSashPosChanged ), NULL, this );
	note->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageChanged ), NULL, this );
	note->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( MainFrame::noteOnAuiNotebookPageClose ), NULL, this );

}
