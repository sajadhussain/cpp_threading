#include "sem.h"

#include <mutex>

Semaphore::Semaphore(int val) :
    value(val),
    wakeups(0)
{
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(mutex);

    if (--value < 0) {
        do {
            cond.wait(lock);
        } while (wakeups < 1);
        --wakeups;
    }
}

void Semaphore::signal()
{
    std::unique_lock<std::mutex> lock(mutex);

    if (++value <= 0) {
        ++wakeups;
        cond.notify_one();
    }
}
