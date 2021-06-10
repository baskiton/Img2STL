#ifndef IMG2STL_LOGGING_H_
#define IMG2STL_LOGGING_H_

#include "wx/ffile.h"
#include "wx/log.h"
#include "wx/string.h"


class Logging : public wxLog {
public:
    explicit Logging(const wxString &t_log_file_name);
    ~Logging() wxOVERRIDE;

    bool is_ok() { return m_log_file.IsOpened() && !m_log_file.Error(); }

protected:
    void DoLogRecord(wxLogLevel t_level,
                     const wxString &t_msg,
                     const wxLogRecordInfo &t_info) wxOVERRIDE;

private:
    void DoLogLine(const wxString &t_level,
                   const wxString &t_timestr,
                   const wxString &t_threadstr,
                   const wxString &t_msg);

    wxLog *m_old_logger;
    wxFFile m_log_file;
};

#endif  // IMG2STL_LOGGING_H_
