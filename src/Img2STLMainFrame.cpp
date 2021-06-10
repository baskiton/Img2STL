#include <map>

#include "Img2STLMainFrame.h"
#include "wx/msgdlg.h"
#include "wx/aboutdlg.h"
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "wx/hashmap.h"
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

Img2STLMainFrame::Img2STLMainFrame(const wxString &t_title)
    :
        MainFrame(nullptr, wxID_ANY, t_title),
        m_files_directory(wxGetCwd()),
        m_exit_files_dir(wxGetCwd()),
        m_executor(nullptr),
        m_logger("log.txt")
    {
    Bind(wxEVT_THREAD, &Img2STLMainFrame::OnThreadUpdate, this);
    wxLogInfo("The program is running.");
}

void Img2STLMainFrame::OnThreadUpdate(wxThreadEvent &event) {
    switch (event.GetId()) {
        case thrCMD_POOL_END:
            m_progress_label->SetLabel("Done!");
            m_progress_label->CentreOnParent(wxHORIZONTAL);
            m_prop_panel->Enable(true);
            m_progress_bar->SetValue(100);
            break;

        case thrCMD_POOL_POINT:
            m_progress_label->SetLabel(event.GetString());
            m_progress_label->CentreOnParent(wxHORIZONTAL);
            break;

        case thrCMD_PBAR_UPD:
            m_progress_bar->SetValue(event.GetInt());
            break;

        default:
            break;
    }
}

void Img2STLMainFrame::OnClose(wxCloseEvent &event) {
    wxLogInfo("Closing the program.");
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
        m_files_directory,        // defaultDir
        wxEmptyString,          // defaultFile
        wxString::Format(       // wildcard
        "Images file (*.bmp *.gif *.png *.jpg)|*.bmp;*.gif;*.png;*.jpg|"
        "All files (%s)|%s",
        wxFileSelectorDefaultWildcardStr,
        wxFileSelectorDefaultWildcardStr),
        wxFD_OPEN | wxFD_MULTIPLE | wxFD_CHANGE_DIR // style
    );

    wxString names = "";

    if (open_file_dlg.ShowModal() == wxID_CANCEL)
        return;

    m_files_directory = open_file_dlg.GetDirectory();
    m_exit_files_dir = m_files_directory;
    open_file_dlg.GetFilenames(m_files_list);
    open_file_dlg.GetFilenames(m_exit_files);

    for (auto &item : m_exit_files) {
        names += item + "; ";
        wxFileName fn(item);
        fn.SetExt("stl");
        item = fn.GetFullName();
    }

    wxLogInfo(wxString::Format("Files %s opened.", names));

    set_prop_exit_label();
    add_image_tabs();

    m_execute_button->Enable(true);
    m_prop_exit_btn->Enable(true);
}

void Img2STLMainFrame::prop_image_autocropOnCheckBox(wxCommandEvent &event) {
    change_image_scale();
}

void Img2STLMainFrame::prop_mask_mode_valOnChoice(wxCommandEvent &event) {
    bool val = m_prop_mask_mode_val->GetSelection();

    m_prop_transp_col_lable->Enable(val);
    m_prop_transp_col_val->Enable(val);

    change_image_scale();
}

void Img2STLMainFrame::prop_transp_col_valOnButtonClick(wxCommandEvent &event) {
    wxColourData col;
    col.SetColour(m_prop_transp_col_val->GetBackgroundColour());
    wxColourDialog col_dlg(this, &col);
    if (col_dlg.ShowModal() == wxID_CANCEL)
        return;

    col.SetColour(col_dlg.GetColourData().GetColour());
    m_prop_transp_col_val->SetBackgroundColour(col.GetColour());
    m_prop_transp_col_val->SetLabel(col.GetColour().GetAsString(wxC2S_HTML_SYNTAX));
    change_image_scale();
}

void Img2STLMainFrame::prop_exit_btnOnButtonClick(wxCommandEvent &event) {
    if (m_files_list.GetCount() == 1) {   // single file
        wxFileDialog save_file_dlg(
            this,
            "Save File",
            m_exit_files_dir,
            m_exit_files[0],
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

        m_exit_files_dir = save_file_dlg.GetDirectory();
    } else if (m_files_list.GetCount() > 1) {     // multi files
        wxDirDialog save_dir_dlg(
            this,
            "Save to directory",
            m_exit_files_dir
        );
        if (save_dir_dlg.ShowModal() == wxID_CANCEL)
            return;

        m_exit_files_dir = save_dir_dlg.GetPath();
    }

    wxLogInfo(wxString::Format("Exit directory changed to %s", m_exit_files_dir));

    set_prop_exit_label();
}

void Img2STLMainFrame::execute_buttonOnButtonClick(wxCommandEvent &event) {
    m_progress_bar->SetValue(0);

    m_executor = new Executor(this, m_files_directory, m_files_list, m_exit_files_dir, m_exit_files, m_prop_image_autocrop->GetValue(),
                              m_prop_den_val->GetValue(), m_prop_max_height_val->GetValue(), m_prop_min_height_val->GetValue(),
                              m_prop_mask_mode_val->GetSelection(), m_prop_transp_col_val->GetBackgroundColour(),
                              m_prop_height_mode_val->GetSelection(),
                              (STLFile::file_type)m_prop_exit_format->GetSelection());

    /** FIXME:
     * at this time setting priority allowed only for windows
    m_executor->SetPriority(wxPRIORITY_DEFAULT);
    //  */

    if (m_executor->Run() != wxTHREAD_NO_ERROR) {
        delete m_executor;
        wxLogError("Can't start Executor thread!");
        return;
    }

    m_prop_panel->Enable(false);
    m_progress_label->SetLabel("Process...");
    m_progress_label->CentreOnParent(wxHORIZONTAL);
}

void Img2STLMainFrame::image_panel_containerOnSize(wxSizeEvent &event) {
    /**
    I don’t understand why yet, but this method is called twice,
    so this is where the "protection" is organized using a counter
    */
    static int cnt = 0;

    if (cnt) {
        cnt = 0;
        m_image_panel_container->SetSashPosition(event.GetSize().GetWidth() - 200);
        change_image_scale();
    } else {
        cnt = 1;
    }
}

void Img2STLMainFrame::image_panel_containerOnSplitterSashPosChanged(wxSplitterEvent &event) {
    m_image_info_panel->SetSize(200, -1);
}

void Img2STLMainFrame::noteOnAuiNotebookPageChanged(wxAuiNotebookEvent &event) {
    if (m_note->GetClientData())
        return;

    change_image_scale();
}

void Img2STLMainFrame::noteOnAuiNotebookPageClose(wxAuiNotebookEvent &event) {
    if (m_note->GetPageCount() <= 0) {
        m_palette_grid->Clear();
        m_palette_grid->Append(new wxPropertyCategory("amount | color"));
    }
}

void Img2STLMainFrame::add_image_tabs() {
    if (m_files_list.GetCount() == 0)
        return;

    m_note->SetClientData((void *)"");

    m_note->DeleteAllPages();

    m_note->SetClientData(nullptr);

    for (const auto &item : m_files_list) {
        auto *tab = new wxPanel(this);
        m_note->AddPage(tab, item);
    }
}

void Img2STLMainFrame::change_image_scale() {
    if (m_files_list.GetCount() == 0)
        return;

    auto cont_size = m_image_container->GetSize();
    double cont_ratio = (double)cont_size.GetWidth() / (double)cont_size.GetHeight();

    wxFileName fn(m_files_directory, m_note->GetPageText(m_note->GetSelection()));

    auto *img = new wxImage(fn.GetFullName());
    if (!img->IsOk())
        return;

    if (m_prop_image_autocrop->GetValue()) {
        wxSize sz;
        wxPoint pt;
        if (get_bound_box(img, sz, pt, m_prop_mask_mode_val->GetSelection(), m_prop_transp_col_val->GetBackgroundColour()))
            img->Resize(sz, pt);
    }

    if (m_prop_mask_mode_val->GetSelection()) {
        wxColour col = m_prop_transp_col_val->GetBackgroundColour();
        img->SetMaskColour(col.Red(), col.Green(), col.Blue());
    }

    if (!img->HasAlpha())
        img->InitAlpha();

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

    auto *tab = m_note->GetCurrentPage();
    tab->DestroyChildren();

    auto *bm = new wxStaticBitmap(tab, wxID_ANY, wxBitmap(*img));
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(bm, 1, wxALL | wxEXPAND, 5);

    delete img;

    tab->SetSizer(sizer);
    tab->Layout();
    sizer->Fit(tab);
    m_note->Layout();
}

void Img2STLMainFrame::set_prop_exit_label() {
    wxFileName fn(m_exit_files_dir, "");
    auto cnt = m_files_list.GetCount();

    if (cnt == 0)
        return;
    else if (cnt == 1)
        fn.SetFullName(m_exit_files[0]);

    m_prop_exit_label->SetLabel(fn.GetFullPath());
    m_prop_exit_label->SetToolTip(fn.GetFullPath());
}

void Img2STLMainFrame::fill_palette(wxImage *t_img) {
    struct color_table {
        unsigned long count;
        wxColour color;
    };

    std::map<unsigned long, color_table> table;

    unsigned char *data = t_img->GetData();
    unsigned char *alpha = t_img->GetAlpha();
    unsigned long size = static_cast<unsigned long>(t_img->GetHeight()) * t_img->GetWidth();

    for (unsigned long i = 0; i < size; ++i) {
        unsigned char r, g, b;
        unsigned long key;

        if (*(alpha++) == 0) {
            data += 3;
            continue;
        }

        r = *(data++);
        g = *(data++);
        b = *(data++);
        key = wxImageHistogram::MakeKey(r, g, b);

        if (table.find(key) == table.end()) {
            table[key] = color_table{1, wxColour(r, g, b)};
        } else {
            table[key].count += 1;
        }
    }

    m_palette_grid->Clear();
    m_palette_grid->Append(new wxPropertyCategory("amount | color"));

    for (const auto& i : table)
        m_palette_grid->Append(new wxColourProperty(wxString::Format("%u", i.second.count), "", i.second.color));

    m_palette_grid->SetSplitterPosition(50);

}
