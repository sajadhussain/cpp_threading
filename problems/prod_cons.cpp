#include "queue.h"

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main(void)
{
    using namespace std::chrono_literals;

    Queue<int> queue(5);
    //std::mutex cout_mutex;
    std::thread producer(
        [&](void)
        {
            for (int i = 0; i < 20; ++i) {
                queue.enqueue(i);
            }
            std::this_thread::sleep_for(7s);
            queue.enqueue(666);
        }
    );
    std::thread consumer(
        [&](void)
        {
            for (int i = 0; i < 20; ++i) {
                int elem = queue.dequeue();
                //cout_mutex.lock();
                std::cout << "removing: " << elem << '\n';
                //cout_mutex.unlock();
            }
        }
    );
    consumer.join();

    auto result = std::async(
        [&](void)
        {
            return queue.dequeue();
        }
    );

    for (int i = 0; i < 5; ++i) {
        auto status = result.wait_for(2s);
        if (status == std::future_status::deferred) {
            std::cout << "deferred\n";
        } else if (status == std::future_status::timeout) {
            std::cout << "timeout\n";
        } else if (status == std::future_status::ready) {
            std::cout << "got: " << result.get() << '\n';
            break;
        }
    }

    if (result.valid()) {
        std::cout << "waiting...\n";
        std::cout << "got: " << result.get() << '\n';
    }

    producer.join();
    return 0;
}
