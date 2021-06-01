//
// Created by baskiton on 07.05.2021.
//

#ifndef IMG2STL_WORKER_H_
#define IMG2STL_WORKER_H_

#include "wx/colour.h"
#include "wx/msgqueue.h"
#include "wx/thread.h"

#include "Geometry.h"
#include "STLFile.h"
#include "WorkerMessage.h"
#include "MessageQueue.h"


class Worker : public wxThread {
public:
    Worker(MessageQueue<WorkerMessage> &t_q_from_exec,
           wxMessageQueue<STLFile> &t_q_to_exec,
           double t_den, int t_exit_type);
    // ~Worker() wxOVERRIDE = default;

protected:
    wxThread::ExitCode Entry() wxOVERRIDE;

    MessageQueue<WorkerMessage> *m_q_from_exec;
    wxMessageQueue<STLFile> *m_q_to_exec;
    float m_density;
    wxColour m_color;
    int m_file_format;
    STLFile m_file;

private:
    void produce(WorkerMessage &t_msg);
    void pixel_calc(WorkerMessage &t_msg);

};


#endif //IMG2STL_WORKER_H_
