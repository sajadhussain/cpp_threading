#ifndef SEM_H
#define SEM_H

#include <condition_variable>
#include <mutex>

class Semaphore {
public:
    Semaphore(int val);
    void wait();
    void signal();

private:
    std::mutex mutex;
    std::condition_variable cond;
    int value;
    int wakeups;
};

#endif
