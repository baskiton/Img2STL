#ifndef IMG2STL_IMG2STLABOUTDIALOG_H_
#define IMG2STL_IMG2STLABOUTDIALOG_H_

#include "wx/icon.h"
#include "wx/string.h"

#include "ui.h"


class Img2STLAboutDialog : public AboutDialog {
public:
    explicit Img2STLAboutDialog(wxWindow *parent);

    void fit();

    void set_icon(const wxIcon &icon);
    void set_name(const wxString &t_name, const wxString &t_version);
    void set_description(const wxString &t_desc);
    void set_using(const wxString &t_using);
    void set_license(const wxString &t_license);
    void set_copyright(const wxString &t_url, const wxString &t_label);
    void add_link(const wxString &t_url, const wxString &t_label);

};

#endif // IMG2STL_IMG2STLABOUTDIALOG_H_
