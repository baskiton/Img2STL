#include "wx/log.h"

#include "Geometry.h"
#include "Worker.h"


Worker::Worker(MessageQueue<WorkerMessage> &t_q_from_exec,
               wxMessageQueue<STLFile> &t_q_to_exec, double t_den)
    :
        wxThread(wxTHREAD_JOINABLE),
        m_q_from_exec(&t_q_from_exec),
        m_q_to_exec(&t_q_to_exec),
        m_density(static_cast<float>(t_den))
    {
    wxLogInfo("Worker created.");
}

Worker::~Worker() {
    wxLogInfo("Worker destroyed.");
}

wxThread::ExitCode Worker::Entry() {
    WorkerMessage msg{};

    wxLogInfo("Worker is running.");

    while (!TestDestroy()) {
        m_q_from_exec->Receive(msg);

        if (msg.cmd == WorkerMessage::wMSG_RUN) {
            produce(msg);
        } else if (msg.cmd == WorkerMessage::wMSG_END) {
            m_q_to_exec->Post(m_file);
            break;
        } else if (msg.cmd == WorkerMessage::wMSG_STOP)
            break;
    }

    return (wxThread::ExitCode)nullptr;
}

void Worker::produce(WorkerMessage &t_msg) {
    pixel_calc(t_msg);
}

void Worker::pixel_calc(WorkerMessage &t_msg) {
    Vertex<float> vx_1{}, vx_2{}, vx_3{}, vx_4{};
    Normal<float> nrm{};

    if (t_msg.z0 != 0) {
        vx_1 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_2 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_3 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_4 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z0)};
        nrm = {0.0f, 0.0f, 1.0f};

        m_file.add(Polygon3<float>{nrm, vx_3, vx_2, vx_4, 0});
        m_file.add(Polygon3<float>{nrm, vx_1, vx_2, vx_3, 0});

        vx_1.z = vx_2.z = vx_3.z = vx_4.z = 0.0f;
        nrm.z = -1.0f;

        m_file.add(Polygon3<float>{nrm, vx_1, vx_3, vx_2, 0});
        m_file.add(Polygon3<float>{nrm, vx_3, vx_4, vx_2, 0});
    }

    if (t_msg.z1 != t_msg.z0) {
        vx_1 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_2 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z1)};
        vx_3 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_4 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z1)};
        nrm = {1.0f, 0.0f, 0.0f};
        if (t_msg.z1 >= t_msg.z0)
            nrm.x = -1.0f;

        m_file.add(Polygon3<float>{nrm, vx_1, vx_2, vx_3, 0});
        m_file.add(Polygon3<float>{nrm, vx_2, vx_4, vx_3, 0});
    }

    if ((t_msg.x == 0) && (t_msg.z0 > 0)) {
        vx_1 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_2 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_3 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                0.0f};
        vx_4 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                0.0f};
        nrm = {-1.0f, 0.0f, 0.0f};

        m_file.add(Polygon3<float>{nrm, vx_1, vx_2, vx_3, 0});
        m_file.add(Polygon3<float>{nrm, vx_1, vx_3, vx_4, 0});
    }

    if (t_msg.z2 != t_msg.z0) {
        vx_1 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_2 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z2)};
        vx_3 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z2)};
        vx_4 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y + 1) * m_density,
                static_cast<float>(t_msg.z0)};
        nrm = {0.0f, 1.0f, 0.0f};
        if (t_msg.z2 >= t_msg.z0)
            nrm.y = -1.0f;

        m_file.add(Polygon3<float>{nrm, vx_1, vx_2, vx_3, 0});
        m_file.add(Polygon3<float>{nrm, vx_1, vx_4, vx_2, 0});
    }

    if ((t_msg.y == 0) && (t_msg.z0 > 0)) {
        vx_1 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_2 = {static_cast<float>(t_msg.x) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                0.0f};
        vx_3 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                static_cast<float>(t_msg.z0)};
        vx_4 = {static_cast<float>(t_msg.x + 1) * m_density,
                static_cast<float>(t_msg.y) * m_density,
                0.0f};
        nrm = {0.0f, -1.0f, 0.0f};

        m_file.add(Polygon3<float>{nrm, vx_1, vx_2, vx_3, 0});
        m_file.add(Polygon3<float>{nrm, vx_2, vx_4, vx_3, 0});
    }
}
