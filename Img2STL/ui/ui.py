# -*- coding: utf-8 -*-

###########################################################################
## Python code generated with wxFormBuilder (version Oct 26 2018)
## http://www.wxformbuilder.org/
##
## PLEASE DO *NOT* EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc
import wx.aui
import wx.propgrid as pg
import wx.adv

###########################################################################
## Class MainFrame
###########################################################################

class MainFrame ( wx.Frame ):

    def __init__( self, title ):
        wx.Frame.__init__ ( self, None, id = wx.ID_ANY, title = title, pos = wx.DefaultPosition, size = wx.Size( 800,600 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

        self.SetSizeHints( wx.Size( 700,530 ), wx.DefaultSize )
        self.SetFont( wx.Font( 9, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, "Noto Sans" ) )

        self.m_menu_bar = wx.MenuBar( 0 )
        self.m_menu_bar.SetFont( wx.Font( 9, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, "Noto Sans" ) )

        self.m_menu_file = wx.Menu()
        self.m_menu_file.AppendSeparator()

        self.m_menu_file_exit = wx.MenuItem( self.m_menu_file, wx.ID_EXIT, u"&Quit", wx.EmptyString, wx.ITEM_NORMAL )
        self.m_menu_file.Append( self.m_menu_file_exit )

        self.m_menu_bar.Append( self.m_menu_file, u"File" )

        self.m_menu_help = wx.Menu()
        self.m_menu_help.AppendSeparator()

        self.m_menu_help_about = wx.MenuItem( self.m_menu_help, wx.ID_ABOUT, u"&About", wx.EmptyString, wx.ITEM_NORMAL )
        self.m_menu_help.Append( self.m_menu_help_about )

        self.m_menu_bar.Append( self.m_menu_help, u"Help" )

        self.SetMenuBar( self.m_menu_bar )

        m_main_sizer = wx.BoxSizer( wx.VERTICAL )

        self.m_main_container = wx.SplitterWindow( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SP_LIVE_UPDATE|wx.SP_NOSASH )
        self.m_main_container.Bind( wx.EVT_IDLE, self.m_main_containerOnIdle )

        self.m_prop_panel = wx.Panel( self.m_main_container, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
        self.m_prop_panel.SetMinSize( wx.Size( 200,-1 ) )

        m_prop_sizer = wx.BoxSizer( wx.VERTICAL )

        m_prop_image_open_sizer = wx.StaticBoxSizer( wx.StaticBox( self.m_prop_panel, wx.ID_ANY, u"Open Image" ), wx.HORIZONTAL )

        self.m_prop_image_open = wx.Button( m_prop_image_open_sizer.GetStaticBox(), wx.ID_OPEN, u"Open Image(s)", wx.DefaultPosition, wx.DefaultSize, 0 )

        self.m_prop_image_open.SetDefault()
        m_prop_image_open_sizer.Add( self.m_prop_image_open, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )

        self.m_prop_image_autocrop = wx.CheckBox( m_prop_image_open_sizer.GetStaticBox(), wx.ID_ANY, u"Auto Crop", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_image_autocrop.SetValue(True)
        m_prop_image_open_sizer.Add( self.m_prop_image_autocrop, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )


        m_prop_sizer.Add( m_prop_image_open_sizer, 0, wx.RIGHT|wx.LEFT|wx.EXPAND, 5 )

        m_prop_prop_sizer = wx.StaticBoxSizer( wx.StaticBox( self.m_prop_panel, wx.ID_ANY, u"Property" ), wx.VERTICAL )

        m_prop_prop_grid = wx.FlexGridSizer( 0, 2, 3, 3 )
        m_prop_prop_grid.SetFlexibleDirection( wx.BOTH )
        m_prop_prop_grid.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )

        self.m_prop_den_lable = wx.StaticText( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"Density (mm/pix)", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_den_lable.Wrap( -1 )

        m_prop_prop_grid.Add( self.m_prop_den_lable, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )

        self.m_prop_den_val = wx.SpinCtrlDouble( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( -1,-1 ), 0, 0, 1000, 2, 0.001 )
        self.m_prop_den_val.SetDigits( 3 )
        m_prop_prop_grid.Add( self.m_prop_den_val, 0, wx.EXPAND|wx.RIGHT|wx.LEFT|wx.ALIGN_CENTER_VERTICAL, 5 )

        self.m_prop_max_height_lable = wx.StaticText( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"Max Height (mm)", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_max_height_lable.Wrap( -1 )

        m_prop_prop_grid.Add( self.m_prop_max_height_lable, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )

        self.m_prop_max_height_val = wx.SpinCtrlDouble( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( -1,-1 ), 0, 0, 1000, 4, 0.001 )
        self.m_prop_max_height_val.SetDigits( 3 )
        m_prop_prop_grid.Add( self.m_prop_max_height_val, 0, wx.RIGHT|wx.LEFT|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )

        self.m_prop_min_height_lable = wx.StaticText( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"Min Height (mm)", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_min_height_lable.Wrap( -1 )

        m_prop_prop_grid.Add( self.m_prop_min_height_lable, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )

        self.m_prop_min_height_val = wx.SpinCtrlDouble( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( -1,-1 ), 0, 0, 1000, 1, 0.001 )
        self.m_prop_min_height_val.SetDigits( 3 )
        m_prop_prop_grid.Add( self.m_prop_min_height_val, 0, wx.RIGHT|wx.LEFT|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )

        self.m_prop_mask_mode_lable = wx.StaticText( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"Mask Mode", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_mask_mode_lable.Wrap( -1 )

        m_prop_prop_grid.Add( self.m_prop_mask_mode_lable, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )

        m_prop_mask_mode_valChoices = [ u"Transparency", u"Color" ]
        self.m_prop_mask_mode_val = wx.Choice( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, wx.DefaultPosition, wx.Size( -1,-1 ), m_prop_mask_mode_valChoices, 0 )
        self.m_prop_mask_mode_val.SetSelection( 0 )
        m_prop_prop_grid.Add( self.m_prop_mask_mode_val, 0, wx.RIGHT|wx.LEFT|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )

        self.m_prop_transp_col_lable = wx.StaticText( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"Transparency color", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_transp_col_lable.Wrap( -1 )

        self.m_prop_transp_col_lable.Enable( False )

        m_prop_prop_grid.Add( self.m_prop_transp_col_lable, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )

        self.m_prop_transp_col_val = wx.Button( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"#FF00FF", wx.DefaultPosition, wx.DefaultSize, wx.BORDER_NONE )
        self.m_prop_transp_col_val.SetBackgroundColour( wx.Colour( 255, 0, 255 ) )
        self.m_prop_transp_col_val.Enable( False )

        m_prop_prop_grid.Add( self.m_prop_transp_col_val, 0, wx.RIGHT|wx.LEFT|wx.ALIGN_CENTER_VERTICAL|wx.EXPAND, 5 )

        self.m_prop_height_mode_lable = wx.StaticText( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, u"Height Mode", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_height_mode_lable.Wrap( -1 )

        m_prop_prop_grid.Add( self.m_prop_height_mode_lable, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )

        m_prop_height_mode_valChoices = [ u"Black", u"White" ]
        self.m_prop_height_mode_val = wx.Choice( m_prop_prop_sizer.GetStaticBox(), wx.ID_ANY, wx.DefaultPosition, wx.Size( -1,-1 ), m_prop_height_mode_valChoices, 0 )
        self.m_prop_height_mode_val.SetSelection( 1 )
        m_prop_prop_grid.Add( self.m_prop_height_mode_val, 0, wx.EXPAND|wx.ALIGN_CENTER_VERTICAL|wx.BOTTOM|wx.RIGHT|wx.LEFT, 5 )


        m_prop_prop_sizer.Add( m_prop_prop_grid, 0, 0, 5 )


        m_prop_sizer.Add( m_prop_prop_sizer, 0, wx.EXPAND|wx.RIGHT|wx.LEFT, 5 )

        m_prop_exit_sizer = wx.StaticBoxSizer( wx.StaticBox( self.m_prop_panel, wx.ID_ANY, u"Exit File" ), wx.VERTICAL )

        m_prop_exit_grid = wx.BoxSizer( wx.HORIZONTAL )

        self.m_prop_exit_label = wx.StaticText( m_prop_exit_sizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_LEFT|wx.ST_ELLIPSIZE_MIDDLE|wx.ST_NO_AUTORESIZE )
        self.m_prop_exit_label.Wrap( -1 )

        m_prop_exit_grid.Add( self.m_prop_exit_label, 1, wx.ALIGN_CENTER_VERTICAL|wx.RIGHT|wx.LEFT, 5 )

        self.m_prop_exit_btn = wx.Button( m_prop_exit_sizer.GetStaticBox(), wx.ID_ANY, u"Choose", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_prop_exit_btn.Enable( False )

        m_prop_exit_grid.Add( self.m_prop_exit_btn, 0, wx.ALIGN_CENTER_VERTICAL|wx.LEFT, 5 )


        m_prop_exit_sizer.Add( m_prop_exit_grid, 0, wx.EXPAND|wx.BOTTOM|wx.RIGHT|wx.LEFT, 5 )

        m_prop_exit_formatChoices = [ u"ASCII", u"BIN" ]
        self.m_prop_exit_format = wx.RadioBox( m_prop_exit_sizer.GetStaticBox(), wx.ID_ANY, u"STL format", wx.DefaultPosition, wx.DefaultSize, m_prop_exit_formatChoices, 1, wx.RA_SPECIFY_ROWS )
        self.m_prop_exit_format.SetSelection( 1 )
        m_prop_exit_sizer.Add( self.m_prop_exit_format, 0, wx.BOTTOM|wx.LEFT, 5 )


        m_prop_sizer.Add( m_prop_exit_sizer, 0, wx.EXPAND|wx.RIGHT|wx.LEFT, 5 )

        m_prop_execute_sizer = wx.StaticBoxSizer( wx.StaticBox( self.m_prop_panel, wx.ID_ANY, u"Execute" ), wx.VERTICAL )

        self.m_execute_button = wx.Button( m_prop_execute_sizer.GetStaticBox(), wx.ID_ANY, u"Execute", wx.DefaultPosition, wx.DefaultSize, 0 )

        self.m_execute_button.SetDefault()
        self.m_execute_button.Enable( False )

        m_prop_execute_sizer.Add( self.m_execute_button, 0, wx.ALIGN_CENTER_HORIZONTAL, 5 )

        self.m_progress_bar = wx.Gauge( m_prop_execute_sizer.GetStaticBox(), wx.ID_ANY, 100, wx.DefaultPosition, wx.DefaultSize, wx.GA_HORIZONTAL|wx.GA_SMOOTH )
        self.m_progress_bar.SetValue( 0 )
        m_prop_execute_sizer.Add( self.m_progress_bar, 0, wx.ALL|wx.EXPAND, 5 )

        self.m_progress_label = wx.StaticText( m_prop_execute_sizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_progress_label.Wrap( -1 )

        m_prop_execute_sizer.Add( self.m_progress_label, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.BOTTOM, 5 )


        m_prop_sizer.Add( m_prop_execute_sizer, 0, wx.EXPAND|wx.BOTTOM|wx.RIGHT|wx.LEFT, 5 )


        self.m_prop_panel.SetSizer( m_prop_sizer )
        self.m_prop_panel.Layout()
        m_prop_sizer.Fit( self.m_prop_panel )
        self.m_imagepanel = wx.ScrolledWindow( self.m_main_container, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.HSCROLL|wx.VSCROLL )
        self.m_imagepanel.SetScrollRate( 5, 5 )
        m_image_panel_sizer = wx.BoxSizer( wx.VERTICAL )

        self.m_image_panel_container = wx.SplitterWindow( self.m_imagepanel, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SP_LIVE_UPDATE|wx.SP_NOSASH )
        self.m_image_container = wx.ScrolledWindow( self.m_image_panel_container, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.BORDER_SUNKEN|wx.HSCROLL|wx.VSCROLL )
        self.m_image_container.SetScrollRate( 5, 5 )
        m_image_sizer = wx.BoxSizer( wx.VERTICAL )

        self.m_note = wx.aui.AuiNotebook( self.m_image_container, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.aui.AUI_NB_CLOSE_ON_ALL_TABS|wx.aui.AUI_NB_SCROLL_BUTTONS|wx.aui.AUI_NB_TAB_MOVE|wx.aui.AUI_NB_TAB_SPLIT|wx.aui.AUI_NB_TOP )

        m_image_sizer.Add( self.m_note, 1, wx.EXPAND, 5 )


        self.m_image_container.SetSizer( m_image_sizer )
        self.m_image_container.Layout()
        m_image_sizer.Fit( self.m_image_container )
        self.m_image_info_panel = wx.ScrolledWindow( self.m_image_panel_container, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_image_info_panel.SetScrollRate( 5, 5 )
        self.m_image_info_panel.SetMinSize( wx.Size( 200,-1 ) )

        m_image_info_panel_sizer = wx.BoxSizer( wx.VERTICAL )

        m_palette_sizer = wx.StaticBoxSizer( wx.StaticBox( self.m_image_info_panel, wx.ID_ANY, u"Palette" ), wx.VERTICAL )

        self.m_palette_grid = pg.PropertyGrid(m_palette_sizer.GetStaticBox(), wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.propgrid.PG_HIDE_MARGIN|wx.propgrid.PG_LIMITED_EDITING)
        self.m_palette_grid.SetMaxSize( wx.Size( 200,-1 ) )

        self.m_palette_header = self.m_palette_grid.Append( pg.PropertyCategory( u"amout | color", u"amout | color" ) )
        m_palette_sizer.Add( self.m_palette_grid, 1, wx.EXPAND, 5 )


        m_image_info_panel_sizer.Add( m_palette_sizer, 1, wx.EXPAND, 5 )


        self.m_image_info_panel.SetSizer( m_image_info_panel_sizer )
        self.m_image_info_panel.Layout()
        m_image_info_panel_sizer.Fit( self.m_image_info_panel )
        self.m_image_panel_container.SplitVertically( self.m_image_container, self.m_image_info_panel, -1 )
        m_image_panel_sizer.Add( self.m_image_panel_container, 1, wx.EXPAND, 5 )


        self.m_imagepanel.SetSizer( m_image_panel_sizer )
        self.m_imagepanel.Layout()
        m_image_panel_sizer.Fit( self.m_imagepanel )
        self.m_main_container.SplitVertically( self.m_prop_panel, self.m_imagepanel, 280 )
        m_main_sizer.Add( self.m_main_container, 1, wx.EXPAND, 5 )


        self.SetSizer( m_main_sizer )
        self.Layout()
        self.m_status_bar = self.CreateStatusBar( 1, wx.STB_SIZEGRIP, wx.ID_ANY )

        self.Centre( wx.BOTH )

        # Connect Events
        self.Bind( wx.EVT_CLOSE, self.OnClose )
        self.Bind( wx.EVT_MENU, self.OnExit, id = self.m_menu_file_exit.GetId() )
        self.Bind( wx.EVT_MENU, self.OnAbout, id = self.m_menu_help_about.GetId() )
        self.m_main_container.Bind( wx.EVT_SPLITTER_SASH_POS_CHANGING, self.main_containerOnSplitterSashPosChanging )
        self.m_prop_image_open.Bind( wx.EVT_BUTTON, self.prop_image_openOnButtonClick )
        self.m_prop_image_autocrop.Bind( wx.EVT_CHECKBOX, self.prop_image_autocropOnCheckBox )
        self.m_prop_mask_mode_val.Bind( wx.EVT_CHOICE, self.prop_mask_mode_valOnChoice )
        self.m_prop_transp_col_val.Bind( wx.EVT_BUTTON, self.prop_transp_col_valOnButtonClick )
        self.m_prop_exit_btn.Bind( wx.EVT_BUTTON, self.prop_exit_btnOnButtonClick )
        self.m_execute_button.Bind( wx.EVT_BUTTON, self.execute_buttonOnButtonClick )
        self.m_image_panel_container.Bind( wx.EVT_SIZE, self.image_panel_containerOnSize )
        self.m_image_panel_container.Bind( wx.EVT_SPLITTER_SASH_POS_CHANGED, self.image_panel_containerOnSplitterSashPosChanged )
        self.m_note.Bind( wx.aui.EVT_AUINOTEBOOK_PAGE_CHANGED, self.noteOnAuiNotebookPageChanged )
        self.m_note.Bind( wx.aui.EVT_AUINOTEBOOK_PAGE_CLOSE, self.noteOnAuiNotebookPageClose )

    def __del__( self ):
        pass


    # Virtual event handlers, overide them in your derived class
    def OnClose( self, event ):
        event.Skip()

    def OnExit( self, event ):
        event.Skip()

    def OnAbout( self, event ):
        event.Skip()

    def main_containerOnSplitterSashPosChanging( self, event ):
        event.Skip()

    def prop_image_openOnButtonClick( self, event ):
        event.Skip()

    def prop_image_autocropOnCheckBox( self, event ):
        event.Skip()

    def prop_mask_mode_valOnChoice( self, event ):
        event.Skip()

    def prop_transp_col_valOnButtonClick( self, event ):
        event.Skip()

    def prop_exit_btnOnButtonClick( self, event ):
        event.Skip()

    def execute_buttonOnButtonClick( self, event ):
        event.Skip()

    def image_panel_containerOnSize( self, event ):
        event.Skip()

    def image_panel_containerOnSplitterSashPosChanged( self, event ):
        event.Skip()

    def noteOnAuiNotebookPageChanged( self, event ):
        event.Skip()

    def noteOnAuiNotebookPageClose( self, event ):
        event.Skip()

    def m_main_containerOnIdle( self, event ):
    	self.m_main_container.SetSashPosition( 280 )
    	self.m_main_container.Unbind( wx.EVT_IDLE )


###########################################################################
## Class AboutDialog
###########################################################################

class AboutDialog ( wx.Dialog ):

    def __init__( self, parent ):
        wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = u"About", pos = wx.DefaultPosition, size = wx.Size( -1,-1 ), style = wx.DEFAULT_DIALOG_STYLE )

        self.SetSizeHints( wx.Size( -1,-1 ), wx.Size( -1,-1 ) )

        self.m_sizer_main = wx.BoxSizer( wx.VERTICAL )

        bSizerInMain = wx.BoxSizer( wx.HORIZONTAL )

        self.m_app_icon = wx.StaticBitmap( self, wx.ID_ANY, wx.NullBitmap, wx.DefaultPosition, wx.Size( 32,32 ), 0 )
        bSizerInMain.Add( self.m_app_icon, 0, wx.ALL, 5 )

        bSizerDesc = wx.BoxSizer( wx.VERTICAL )

        bSizerName = wx.BoxSizer( wx.HORIZONTAL )


        bSizerName.Add( ( 0, 0), 1, wx.EXPAND, 5 )

        self.m_name_version = wx.StaticText( self, wx.ID_ANY, u"%name% %version%", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_name_version.Wrap( -1 )

        self.m_name_version.SetFont( wx.Font( 12, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )

        bSizerName.Add( self.m_name_version, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )


        bSizerName.Add( ( 0, 0), 1, wx.EXPAND, 5 )


        bSizerDesc.Add( bSizerName, 1, wx.EXPAND, 5 )

        self.m_description = wx.StaticText( self, wx.ID_ANY, u"%description%", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_description.Wrap( -1 )

        bSizerDesc.Add( self.m_description, 0, wx.ALL|wx.EXPAND, 5 )

        self.m_using_str = wx.StaticText( self, wx.ID_ANY, u"Using %wx%", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_using_str.Wrap( -1 )

        bSizerDesc.Add( self.m_using_str, 0, wx.EXPAND|wx.ALL, 5 )

        self.m_url_sizer = wx.BoxSizer( wx.VERTICAL )


        bSizerDesc.Add( self.m_url_sizer, 1, wx.EXPAND, 5 )


        bSizerInMain.Add( bSizerDesc, 1, wx.EXPAND, 5 )


        self.m_sizer_main.Add( bSizerInMain, 0, wx.EXPAND, 5 )

        self.m_license_pane = wx.CollapsiblePane( self, wx.ID_ANY, u"License", wx.DefaultPosition, wx.DefaultSize, wx.CP_DEFAULT_STYLE )
        self.m_license_pane.Collapse( True )

        self.m_sizer_license = wx.BoxSizer( wx.VERTICAL )


        self.m_license_pane.GetPane().SetSizer( self.m_sizer_license )
        self.m_license_pane.GetPane().Layout()
        self.m_sizer_license.Fit( self.m_license_pane.GetPane() )
        self.m_sizer_main.Add( self.m_license_pane, 1, wx.EXPAND|wx.ALL, 5 )

        bSizerLow = wx.BoxSizer( wx.HORIZONTAL )

        self.m_copyright = wx.adv.HyperlinkCtrl( self, wx.ID_ANY, u"%label%", u"%url%", wx.DefaultPosition, wx.DefaultSize, wx.adv.HL_DEFAULT_STYLE )
        bSizerLow.Add( self.m_copyright, 1, wx.ALL|wx.ALIGN_BOTTOM, 5 )

        m_sdbSizer = wx.StdDialogButtonSizer()
        self.m_sdbSizerOK = wx.Button( self, wx.ID_OK )
        m_sdbSizer.AddButton( self.m_sdbSizerOK )
        m_sdbSizer.Realize();

        bSizerLow.Add( m_sdbSizer, 0, wx.EXPAND|wx.ALL, 5 )


        self.m_sizer_main.Add( bSizerLow, 0, wx.EXPAND, 5 )


        self.SetSizer( self.m_sizer_main )
        self.Layout()
        self.m_sizer_main.Fit( self )

        self.Centre( wx.BOTH )

    def __del__( self ):
        pass


