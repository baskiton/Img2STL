#include "wx/image.h"
#include "wx/log.h"
#include "wx/msgqueue.h"

#include "Executor.h"
#include "Img2STLMainFrame.h"
#include "ImgExtFunc.h"
#include "ThreadCommands.h"
#include "Worker.h"

Executor::Executor(
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
        const wxColour &t_mask_color,
        int t_height_mode,
        STLFile::file_type t_f_type
    )
    :
        wxThread(wxTHREAD_DETACHED),
        m_main_evt_handler(t_evt_hdlr),
        m_files_dir(t_files_dir),
        m_files_list(t_files_list),
        m_exit_files_dir(t_exit_dir),
        m_exit_files_list(t_exit_list),
        m_autocrop(t_autocrop),
        m_density(t_density),
        m_height_max(t_height_max),
        m_height_min(t_height_min),
        m_mask_mode(t_mask_mode),
        m_mask_color(t_mask_color),
        m_height_mode(t_height_mode),
        m_f_type(t_f_type)
    {
}

wxThread::ExitCode Executor::Entry() {
    if (m_files_list.GetCount() == 0) {
        m_event.SetId(thrCMD_POOL_END);
        wxQueueEvent(m_main_evt_handler, m_event.Clone());

        return (wxThread::ExitCode)nullptr;
    }

    auto points_per_file = 100.0 / static_cast<double>(m_files_list.GetCount());
    double pb_val = 0.0;
    int cpu_cnt = wxThread::GetCPUCount();
    if (cpu_cnt < 0)
        cpu_cnt = 1;

    double mod = m_height_mode ? m_height_min : m_height_max;
    double height_factor = 255.0 / (m_height_max - m_height_min);

    if (!m_height_mode)
        height_factor *= -1;

    for (const auto &img_name : m_files_list) {
        auto idx = &img_name - m_files_list.begin();
        if (TestDestroy())
            break;

        m_event.SetId(thrCMD_POOL_POINT);
        m_event.SetString(img_name);
        wxQueueEvent(m_main_evt_handler, m_event.Clone());

        for (auto i = 0; i < cpu_cnt; ++i) {
            auto *thr = new Worker(m_q_to_worker, m_q_from_worker, m_density, m_f_type);

            /*
             * at this time setting priority allowed only for windows
            thr->SetPriority(wxPRIORITY_MIN);
            //  */

            if (thr->Run() != wxTHREAD_NO_ERROR) {
                wxLogError("Can't start worker thread!");
                continue;
            }
            m_thr_pool.Add(thr);
        }

        if (!m_thr_pool.GetCount()) {
            /** TODO: some error - thread list is empty. terminating. Notify main thread */
            wxLogError("No worker threads have been created!");
            break;
        }

        if (TestDestroy())
            break;

        wxImage bw(wxFileName(m_files_dir, img_name).GetFullPath());

        if (m_autocrop) {
            wxSize sz;
            wxPoint pt;
            if (get_bound_box(&bw, sz, pt, m_mask_mode, m_mask_color))
                bw.Resize(sz, pt);
        }

        img_to_heightmap(bw, m_mask_mode, m_mask_color);

        if (!bw.HasAlpha())
            bw.InitAlpha();

        int img_h = bw.GetHeight();
        int img_w = bw.GetWidth();
        auto tot_count = img_h * (long)img_w;
        m_q_to_worker.Clear();

        for (auto y = 0; (y < img_h) && !TestDestroy(); ++y) {
            for (auto x = 0; x < img_w; ++x) {
                double z0 = 0,
                       z1 = 0,
                       z2 = 0;
                if (bw.GetAlpha(x, y)) {
                    z0 = bw.GetRed(x, y) / height_factor + mod;
                }
                if (((x + 1) < img_w) && bw.GetAlpha(x + 1, y)) {
                    z1 = bw.GetRed(x + 1, y) / height_factor + mod;
                }
                if (((y + 1) < img_h) && bw.GetAlpha(x, y + 1)) {
                    z2 = bw.GetRed(x, y + 1) / height_factor + mod;
                }

                m_q_to_worker.Post(WorkerMessage{WorkerMessage::wMSG_RUN, x, y, z0, z1, z2});
            }
        }

        for (auto i = 0; i < m_thr_pool.GetCount(); ++i)
            m_q_to_worker.Post(WorkerMessage{WorkerMessage::wMSG_END});

        while (!TestDestroy()) {
            auto i = m_q_to_worker.GetCount();
            int val = int(pb_val + points_per_file * (1 - ((double)i / (double)tot_count)));
            m_event.SetId(thrCMD_PBAR_UPD);
            m_event.SetInt(val);
            wxQueueEvent(m_main_evt_handler, m_event.Clone());
            if (i == 0) {
                pb_val += points_per_file;
                break;
            }
             Sleep(100);
        }

        if (!TestDestroy()) {
            int cnt = 0;
            for (auto *t : m_thr_pool)
                t->Wait();

            for (auto *t : m_thr_pool) {
                delete t;
                ++cnt;
            }

            m_thr_pool.Clear();

            generate_stl(cnt, idx);
        }
    }

    m_event.SetId(thrCMD_POOL_END);
    wxQueueEvent(m_main_evt_handler, m_event.Clone());

    return (wxThread::ExitCode)nullptr;
}

Executor::~Executor() {
    wxCriticalSectionLocker enter(m_main_evt_handler->m_executor_CS);
    m_main_evt_handler->m_executor = nullptr;
}

void Executor::OnDelete() {
    m_q_to_worker.Clear();

    for ([[maybe_unused]] auto t : m_thr_pool)
        m_q_to_worker.Post(WorkerMessage{WorkerMessage::wMSG_STOP});

    for (auto *t : m_thr_pool)
        t->Delete();

    for (auto *t : m_thr_pool)
        delete t;

    m_thr_pool.Clear();
}

void Executor::generate_stl(int t_cnt, std::size_t idx) {
    wxString &f_name = m_exit_files_list.Item(idx);
    STLFile result_file(f_name);
    STLFile msg;

    if (m_f_type == STLFile::BIN) {
        result_file.set_header(wxString::Format("Generate from Img2STL by baskiton. %s", f_name));
    }

    for (auto i = 0; i < t_cnt; ++i) {
        m_q_from_worker.Receive(msg);
        result_file.add(msg);
    }

    result_file.save_file(m_f_type, wxFileName(m_exit_files_dir, f_name));
}
