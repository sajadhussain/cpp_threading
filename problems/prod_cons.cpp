/*
    C++ threading examples
    Copyright (C) 2018 Yağmur Oymak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
