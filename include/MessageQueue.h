#ifndef IMG2STL_MESSAGEQUEUE_H_
#define IMG2STL_MESSAGEQUEUE_H_

#include "wx/thread.h"

#if wxUSE_THREADS

#include "wx/msgqueue.h"

#include "WorkerMessage.h"


template<typename T>
class MessageQueue {
public:
    MessageQueue() : m_conditionNotEmpty(m_mutex) {}
    MessageQueue(const MessageQueue<T>& rhs) = delete;
    MessageQueue<T>& operator=(const MessageQueue<T>& rhs) = delete;

    std::size_t GetCount() const {
        wxMutexLocker locker(m_mutex);

        wxCHECK(locker.IsOk(), wxMSGQUEUE_MISC_ERROR);

        return m_messages.size();
    }

    wxMessageQueueError Post(const T &msg) {
        wxMutexLocker locker(m_mutex);

        wxCHECK(locker.IsOk(), wxMSGQUEUE_MISC_ERROR);
        m_messages.push(msg);
        m_conditionNotEmpty.Signal();

        return wxMSGQUEUE_NO_ERROR;
    }

    wxMessageQueueError Clear() {
        wxCHECK(IsOk(), wxMSGQUEUE_MISC_ERROR);

        wxMutexLocker locker(m_mutex);

        std::queue<T> empty;
        std::swap(m_messages, empty);

        return wxMSGQUEUE_NO_ERROR;
    }

    wxMessageQueueError Receive(T &msg) {
        wxCHECK(IsOk(), wxMSGQUEUE_MISC_ERROR);

        wxMutexLocker locker(m_mutex);

        wxCHECK(locker.IsOk(), wxMSGQUEUE_MISC_ERROR);

        while (m_messages.empty()) {
            wxCondError result = m_conditionNotEmpty.Wait();
            wxCHECK(result == wxCOND_NO_ERROR, wxMSGQUEUE_MISC_ERROR);
        }

        msg = m_messages.front();
        m_messages.pop();

        return wxMSGQUEUE_NO_ERROR;
    }

    bool IsOk() const { return m_conditionNotEmpty.IsOk(); }

private:
    mutable wxMutex m_mutex;
    wxCondition m_conditionNotEmpty;

    std::queue<T> m_messages;
};

#endif // wxUSE_THREADS

#endif // IMG2STL_MESSAGEQUEUE_H_
