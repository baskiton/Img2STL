#include "wx/datetime.h"
#include "wx/filename.h"

#include "Logging.h"


Logging::Logging(const wxString &t_log_file_name)
    :
        m_old_logger(wxLog::GetActiveTarget()),
        m_log_file(t_log_file_name, "w+")
    {
    wxLog::SetActiveTarget(this);

    if (is_ok()) {
        auto session_str = wxString::Format("Session started at %s\n", wxDateTime::Now().FormatISODate());
        m_log_file.Write(session_str.c_str(), session_str.length());
        m_log_file.Sync();
    }
    wxLogInfo("=== LOGGING STARTED ===");
}

Logging::~Logging() {
    wxLogInfo("=== LOGGING FINISHED ===");
    m_log_file.Close();
    wxLog::SetActiveTarget(m_old_logger);
}


void Logging::DoLogLine(const wxString &t_level,
                        const wxString &t_timestr,
                        const wxString &t_threadstr,
                        const wxString &t_msg) {
    if (is_ok()) {
        auto t_str = wxString::Format("<%8s> %8s [%10s] %s", t_level, t_timestr, t_threadstr, t_msg);
        m_log_file.Write(t_str.c_str(), t_str.length());
        m_log_file.Sync();
    }
}

void Logging::DoLogRecord(wxLogLevel t_level,
                          const wxString &t_msg,
                          const wxLogRecordInfo &t_info) {
    wxString level_str;
    wxString postfix = "";

    if (t_level <= wxLOG_Warning || t_level == wxLOG_Trace || t_level == wxLOG_Debug) {
        postfix = wxString::Format(" [%s() in %s(%d)]", t_info.func, wxFileName(t_info.filename).GetFullName(), t_info.line);
        m_old_logger->LogRecord(t_level, t_msg, t_info);
    }

    switch (t_level) {
        case wxLOG_Error:
            level_str = "ERROR";
            break;

        case wxLOG_Warning:
            level_str = "WARNING";
            break;

        case wxLOG_Message:
            level_str = "MESSAGE";
            break;

        case wxLOG_Status:
            level_str = "STATUS";
            break;

        case wxLOG_Info:
            level_str = "INFO";
            break;

        case wxLOG_Debug: 
            level_str = "DEBUG";
            break;

        case wxLOG_Trace:
            level_str = "TRACE";
            break;

        case wxLOG_Progress:
            level_str = "PROGRESS";
            break;

        default:
            level_str = "USER";
    }

    DoLogLine(
        level_str,
        wxDateTime(wxLongLong(t_info.timestampMS)).FormatISOTime(),
        t_info.threadId == wxThread::GetMainId()
            ? wxString("main")
            : wxString::Format("%lx", t_info.threadId),
        wxString::Format("%s%s\n", t_msg, postfix)
    );
}
