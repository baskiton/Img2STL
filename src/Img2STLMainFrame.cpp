#include "Img2STLMainFrame.h"
#include "wx/msgdlg.h"
#include "wx/aboutdlg.h"
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "wx/statbmp.h"
#include "wx/quantize.h"
#include "wx/colordlg.h"
#include "wx/msgqueue.h"

#include "Executor.h"
#include "ImgExtFunc.h"
#include "ThreadCommands.h"


//helper functions
enum wxbuildinfoformat {
    short_f,
    long_f
};

wxString wxbuildinfo(wxbuildinfoformat format) {
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f){
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

Img2STLMainFrame::Img2STLMainFrame(const wxString &title)
    :
        MainFrame(nullptr, wxID_ANY, title),
        files_directory(wxGetCwd()),
        exit_files_dir(wxGetCwd()),
        m_executor(nullptr)
    {
    Bind(wxEVT_THREAD, &Img2STLMainFrame::OnThreadUpdate, this);
}

void Img2STLMainFrame::OnThreadUpdate(wxThreadEvent &event) {
    switch (event.GetId()) {
        case thrCMD_POOL_END:
            progress_label->SetLabel("Done!");
            progress_label->CentreOnParent(wxHORIZONTAL);
            prop_panel->Enable(true);
            progress_bar->SetValue(100);
            break;

        case thrCMD_POOL_POINT:
            progress_label->SetLabel(event.GetString());
            progress_label->CentreOnParent(wxHORIZONTAL);
            break;

        case thrCMD_PBAR_UPD:
            progress_bar->SetValue(event.GetInt());
            break;

        default:
            break;
    }
}

void Img2STLMainFrame::OnClose(wxCloseEvent &event) {
    {
        wxCriticalSectionLocker enter(m_executor_CS);
        if (m_executor) {
            if (m_executor->Delete() != wxTHREAD_NO_ERROR)
                wxLogError("Can't delete the thread!");
        }
    }
    while (true) {
        {
            wxCriticalSectionLocker enter(m_executor_CS);
            if (!m_executor)
                break;
        }
        wxThread::Sleep(1);
    }

    Destroy();
}

void Img2STLMainFrame::OnExit(wxCommandEvent &event) {
    Close();
    // Close(true);
}

void Img2STLMainFrame::OnAbout(wxCommandEvent &event) {
    wxAboutDialogInfo info;
    info.AddDeveloper("Alexander Baskikh aka baskiton");
    info.SetName("Image to STL");
    info.SetVersion("2.0.0-alpha");
    info.SetDescription(
            wxString::Format(
                    "%s\n\n"
                    "Developed by baskiton",
                    wxbuildinfo(long_f)
            )
    );
    info.SetLicence("MIT");
    info.SetWebSite("vk.com/baskiton", "VK");
    wxAboutBox(info, this);
}

void Img2STLMainFrame::main_containerOnSplitterSashPosChanging(wxSplitterEvent &event) {
    Layout();
}

void Img2STLMainFrame::prop_image_openOnButtonClick(wxCommandEvent &event) {
    wxFileDialog open_file_dlg(
        this,                   // parent
        "Open image files",     // message
        files_directory,        // defaultDir
        wxEmptyString,          // defaultFile
        wxString::Format(       // wildcard
        "Images file (*.bmp *.gif *.png *.jpg)|*.bmp;*.gif;*.png;*.jpg|"
        "All files (%s)|%s",
        wxFileSelectorDefaultWildcardStr,
        wxFileSelectorDefaultWildcardStr),
        wxFD_OPEN | wxFD_MULTIPLE | wxFD_CHANGE_DIR // style
    );

    if (open_file_dlg.ShowModal() == wxID_CANCEL)
        return;

    files_directory = open_file_dlg.GetDirectory();
    exit_files_dir = files_directory;
    open_file_dlg.GetFilenames(files_list);
    open_file_dlg.GetFilenames(exit_files);

    for (auto &item : exit_files) {
        wxFileName fn(item);
        fn.SetExt("stl");
        item = fn.GetFullName();
//        wxMessageDialog dg(this, item, exit_files_dir);
//        dg.ShowModal();
    }

    set_prop_exit_label();
    add_image_tabs();
    // change_image_scale();

    execute_button->Enable(true);
    prop_exit_btn->Enable(true);
}

void Img2STLMainFrame::prop_image_autocropOnCheckBox(wxCommandEvent &event) {
    change_image_scale();
}

void Img2STLMainFrame::prop_mask_mode_valOnChoice(wxCommandEvent &event) {
    bool val = prop_mask_mode_val->GetSelection();

    prop_transp_col_lable->Enable(val);
    prop_transp_col_val->Enable(val);

    change_image_scale();
}

void Img2STLMainFrame::prop_transp_col_valOnButtonClick(wxCommandEvent &event) {
    wxColourData col;
    col.SetColour(prop_transp_col_val->GetBackgroundColour());
    wxColourDialog col_dlg(this, &col);
    if (col_dlg.ShowModal() == wxID_CANCEL)
        return;

    col.SetColour(col_dlg.GetColourData().GetColour());
    prop_transp_col_val->SetBackgroundColour(col.GetColour());
    prop_transp_col_val->SetLabel(col.GetColour().GetAsString(wxC2S_HTML_SYNTAX));
    change_image_scale();
}

void Img2STLMainFrame::prop_exit_btnOnButtonClick(wxCommandEvent &event) {
    if (files_list.GetCount() == 1) {   // single file
        wxFileDialog save_file_dlg(
            this,
            "Save File",
            exit_files_dir,
            exit_files[0],
            wxString::Format(       // wildcard
                "STL file (*.stl)|*.stl|"
                "All files (%s)|%s",
                wxFileSelectorDefaultWildcardStr,
                wxFileSelectorDefaultWildcardStr
            ),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT
        );
        if (save_file_dlg.ShowModal() == wxID_CANCEL)
            return;

        exit_files_dir = save_file_dlg.GetDirectory();
    } else if (files_list.GetCount() > 1) {     // multi files
        wxDirDialog save_dir_dlg(
            this,
            "Save to directory",
            exit_files_dir
        );
        if (save_dir_dlg.ShowModal() == wxID_CANCEL)
            return;

        exit_files_dir = save_dir_dlg.GetPath();
    }

    set_prop_exit_label();
}

void Img2STLMainFrame::execute_buttonOnButtonClick(wxCommandEvent &event) {
// TODO: Implement execute_buttonOnButtonClick

    progress_bar->SetValue(0);

    m_executor = new Executor(this, files_directory, files_list, exit_files_dir, exit_files, prop_image_autocrop->GetValue(),
                              prop_den_val->GetValue(), prop_max_height_val->GetValue(), prop_min_height_val->GetValue(),
                              prop_mask_mode_val->GetSelection(), prop_transp_col_val->GetBackgroundColour(),
                              prop_height_mode_val->GetSelection(),
                              (STLFile::file_type)prop_exit_format->GetSelection());

    /*
     * at this time setting priority allowed only for windows
    m_executor->SetPriority(wxPRIORITY_DEFAULT);
    //  */

    if (m_executor->Run() != wxTHREAD_NO_ERROR) {
        delete m_executor;
        // m_executor = nullptr;
        wxMessageDialog dg(this, "Can't start Executor thread!");
        dg.ShowModal();
        wxLogError("Can't start Executor thread!");
        return;
    }

    prop_panel->Enable(false);
    progress_label->SetLabel("Process...");
    progress_label->CentreOnParent(wxHORIZONTAL);
}

void Img2STLMainFrame::image_panel_containerOnSize(wxSizeEvent &event) {
    /**
    I don’t understand why yet, but this method is called twice,
    so this is where the "protection" is organized using a counter
    */
    static int cnt = 0;

    if (cnt) {
        cnt = 0;
        image_panel_container->SetSashPosition(event.GetSize().GetWidth() - 200);
        // statusBar->SetStatusText(wxString::Format("Sash Pos: %d", image_panel_container->GetSashPosition()));
        change_image_scale();
    } else {
        cnt = 1;
    }
}

void Img2STLMainFrame::image_panel_containerOnSplitterSashPosChanged(wxSplitterEvent &event) {
    image_info_panel->SetSize(200, -1);
    // change_image_scale();
}

void Img2STLMainFrame::noteOnAuiNotebookPageChanged(wxAuiNotebookEvent &event) {
    if (note->GetClientData())
        return;

    change_image_scale();
}

void Img2STLMainFrame::noteOnAuiNotebookPageClose(wxAuiNotebookEvent &event) {
//    wxMessageDialog dg(this, note->GetPageText(event.GetOldSelection()));
//    dg.ShowModal();

    if (note->GetPageCount() <= 0) {
        palette_grid->Clear();
        palette_grid->Append(new wxPropertyCategory("amount | color"));
    }
}

void Img2STLMainFrame::add_image_tabs() {
    if (files_list.GetCount() == 0)
        return;

    note->SetClientData((void *)"");

    note->DeleteAllPages();

    note->SetClientData(nullptr);

    for (const auto &item : files_list) {
        auto *tab = new wxPanel(this);
        note->AddPage(tab, item);
    }
}

void Img2STLMainFrame::change_image_scale() {

    if (files_list.GetCount() == 0)
        return;

    auto cont_size = image_container->GetSize();
    double cont_ratio = (double)cont_size.GetWidth() / (double)cont_size.GetHeight();

    wxFileName fn(files_directory, note->GetPageText(note->GetSelection()));

    auto *img = new wxImage(fn.GetFullName());
    if (!img->IsOk())
        return;

//    wxMessageDialog dg(this, wxString::Format("%ix%i", img->GetWidth(), img->GetHeight()));
//    dg.ShowModal();

    if (prop_image_autocrop->GetValue()) {
        wxSize sz;
        wxPoint pt;
        if (get_bound_box(img, sz, pt, prop_mask_mode_val->GetSelection(), prop_transp_col_val->GetBackgroundColour()))
            img->Resize(sz, pt);
    }

    if (prop_mask_mode_val->GetSelection()) {
        wxColour col = prop_transp_col_val->GetBackgroundColour();
        img->SetMaskColour(col.Red(), col.Green(), col.Blue());
    }

    fill_palette(img);

    double w, h;
    double img_ratio = (double)img->GetWidth() / (double)img->GetHeight();

    if (cont_ratio > img_ratio) {
        w = (img->GetWidth() * cont_size.GetHeight()) / (double)img->GetHeight() * 0.8;
        h = cont_size.GetHeight() * 0.8;
    } else {
        w = cont_size.GetWidth() * 0.8;
        h = (img->GetHeight() * cont_size.GetWidth()) / (double)img->GetWidth() * 0.8;
    }

    img->Rescale((int)w, (int)h);

    auto *tab = note->GetCurrentPage();
    tab->DestroyChildren();

    auto *bm = new wxStaticBitmap(tab, wxID_ANY, wxBitmap(*img));
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(bm, 1, wxALL | wxEXPAND, 5);

    delete img;

    tab->SetSizer(sizer);
    tab->Layout();
    sizer->Fit(tab);
    note->Layout();
//    image_container->Layout();
//    image_panel_container->Layout();
}

void Img2STLMainFrame::set_prop_exit_label() {
    wxFileName fn(exit_files_dir, "");
    auto cnt = files_list.GetCount();

    if (cnt == 0)
        return;
    else if (cnt == 1)
        fn.SetFullName(exit_files[0]);

    prop_exit_label->SetLabel(fn.GetFullPath());
    prop_exit_label->SetToolTip(fn.GetFullPath());
}

void Img2STLMainFrame::fill_palette(wxImage *img) {
    // wxPalette palette = img->GetPalette();
    // wxPalette *pt = &palette;
    auto *pt = (wxPalette *)&img->GetPalette();

    struct colors {
        int count;
        wxColour color;
    };

    std::vector<colors> col_list;
    int col_cnt;

    if (pt->IsOk()) {
        col_cnt = pt->GetColoursCount();
    } else {
        wxImage img_tmp;
        col_cnt = img->CountColours() > 256 ? 256 : (int)img->CountColours();
        wxQuantize::Quantize(*img, img_tmp, &pt, (col_cnt > 256 ? 256 : col_cnt), nullptr, 0);
        // unsigned char* red = new unsigned char[256];
        // unsigned char* green = new unsigned char[256];
        // unsigned char* blue = new unsigned char[256];
        // for (size_t i = 0; i < 256; i ++)
        //     red[i] = green[i] = blue[i] = 0;
        // wxPalette palette(col_cnt, red, green, blue);
        // img->SetPalette(palette);
        // img->SetOption(wxIMAGE_OPTION_BMP_FORMAT, wxBMP_8BPP_PALETTE);
    }

    for (int i = 0; i < col_cnt; ++i) {
        unsigned char r, g, b;
        pt->GetRGB(i, &r, &g, &b);
        col_list.push_back({0, wxColour(r, g, b)});
    }

    palette_grid->Clear();
    palette_grid->Append(new wxPropertyCategory("amount | color"));

    // for (int i = 0; i < pt->GetColoursCount(); ++i) {
    //     unsigned char r, g, b;
    //     pt->GetRGB(i, &r, &g, &b);
    //     wxColour color(r, g, b);

    //     palette_grid->Append(new wxColourProperty(wxString::Format("%i", i), wxPG_LABEL, color));
    // }

    for (const auto& i : col_list)
        palette_grid->Append(new wxColourProperty(wxString::Format("%i", i.count), "", i.color));

    palette_grid->SetSplitterPosition(50);

}
