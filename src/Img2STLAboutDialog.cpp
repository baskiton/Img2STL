#include "wx/textctrl.h"

#include "Img2STLAboutDialog.h"


Img2STLAboutDialog::Img2STLAboutDialog(wxWindow *parent)
    :
        AboutDialog(parent)
    {
}

void Img2STLAboutDialog::fit() {
    this->Layout();
    m_sizer_main->Fit(this);
}

void Img2STLAboutDialog::set_icon(const wxIcon &t_icon) {
    m_app_icon->SetIcon(t_icon);
}

void Img2STLAboutDialog::set_name(const wxString &t_name, const wxString &t_version) {
    m_name_version->SetLabel(wxString::Format("%s %s", t_name, t_version));
}

void Img2STLAboutDialog::set_description(const wxString &t_desc) {
    m_description->SetLabel(t_desc);
}

void Img2STLAboutDialog::set_using(const wxString &t_using) {
    m_using_str->SetLabel(t_using);
}

void Img2STLAboutDialog::set_license(const wxString &t_license) {
    auto *pane = m_license_pane->GetPane();
    auto *license = new wxTextCtrl(pane, wxID_ANY, t_license, wxDefaultPosition, wxDefaultSize,
                                   wxTE_BESTWRAP | wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP);

    m_sizer_license->Add(license, 1, wxEXPAND, 5);

    pane->SetSizer(m_sizer_license);
    pane->Layout();
    m_sizer_license->Fit(pane);
}

void Img2STLAboutDialog::set_copyright(const wxString &t_url, const wxString &t_label) {
    m_copyright->SetURL(t_url);
    m_copyright->SetLabel(t_label);
}

void Img2STLAboutDialog::add_link(const wxString &t_url, const wxString &t_label) {
    auto *url = new wxHyperlinkCtrl(this, wxID_ANY, t_label, t_url);
    m_url_sizer->Add(url, 0, wxALL, 5);
}
