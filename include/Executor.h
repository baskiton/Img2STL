#ifndef IMG2STL_EXECUTOR_H_
#define IMG2STL_EXECUTOR_H_

#include "wx/colour.h"
#include "wx/event.h"
#include "wx/filename.h"
#include "wx/image.h"
#include "wx/msgqueue.h"
#include "wx/thread.h"

#include "STLFile.h"
#include "MessageQueue.h"


WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);


class Img2STLMainFrame;

class Executor : public wxThread {
public:
    Executor(
        Img2STLMainFrame *t_evt_hdlr,
        const wxString &t_files_dir,
        const wxArrayString &t_files_list,
        const wxString &t_exit_dir,
        const wxArrayString &t_exit_list,
        bool t_autocrop,
        double t_density,
        double t_height_max,
        double t_height_min,
        int t_mask_mode,
        wxColour t_mask_color,
        int t_height_mode,
        STLFile::file_type t_f_type
    );
    ~Executor() wxOVERRIDE;

protected:
    wxThread::ExitCode Entry() wxOVERRIDE;
    void OnDelete() wxOVERRIDE;

    void generate_stl(int t_cnt, std::size_t idx);

private:
    Img2STLMainFrame *m_main_evt_handler;
    wxThreadEvent m_event;
    wxArrayThread m_thr_pool;
    MessageQueue<WorkerMessage> m_q_to_worker;
    wxMessageQueue<STLFile> m_q_from_worker;

    wxString m_files_dir;
    wxArrayString m_files_list;
    wxString m_exit_files_dir;
    wxArrayString m_exit_files_list;

    bool m_autocrop;
    double m_density;
    double m_height_max;
    double m_height_min;
    int m_mask_mode;
    wxColour m_mask_color;
    int m_height_mode;
    STLFile::file_type m_f_type;

};


#endif // IMG2STL_EXECUTOR_H_
