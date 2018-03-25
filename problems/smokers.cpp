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

#include "sem.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

auto agent_sem = Semaphore(1);
auto tobacco = Semaphore(0);
auto paper = Semaphore(0);
auto match = Semaphore(0);

auto is_tobacco = false;
auto is_paper = false;
auto is_match = false;
auto tobacco_sem = Semaphore(0);
auto paper_sem = Semaphore(0);
auto match_sem = Semaphore(0);
auto pusher_mutex = std::mutex();

auto cout_mutex = std::mutex();

void agent_a(void)
{
    for (;;) {
        agent_sem.wait();
        tobacco.signal();
        paper.signal();
    }
}

void agent_b(void)
{
    for (;;) {
        agent_sem.wait();
        paper.signal();
        match.signal();
    }
}

void agent_c(void)
{
    for (;;) {
        agent_sem.wait();
        tobacco.signal();
        match.signal();
    }
}

void pusher_tobacco(void)
{
    for (;;) {
        tobacco.wait();

        std::unique_lock<std::mutex> lock(pusher_mutex);
        if (is_match) {
            is_match = false;
            paper_sem.signal();
        } else if (is_paper) {
            is_paper = false;
            match_sem.signal();
        } else {
            is_tobacco = true;
        }
    }
}

void pusher_match(void)
{
    for (;;) {
        match.wait();

        std::unique_lock<std::mutex> lock(pusher_mutex);
        if (is_paper) {
            is_paper = false;
            tobacco_sem.signal();
        } else if (is_tobacco) {
            is_tobacco = false;
            paper_sem.signal();
        } else {
            is_match = true;
        }
    }
}

void pusher_paper(void)
{
    for (;;) {
        paper.wait();

        std::unique_lock<std::mutex> lock(pusher_mutex);
        if (is_match) {
            is_match = false;
            tobacco_sem.signal();
        } else if (is_tobacco) {
            is_tobacco = false;
            match_sem.signal();
        } else {
            is_paper = true;
        }
    }
}

void smoker_matches(void)
{
    for (;;) {
        match_sem.wait();
        agent_sem.signal();

        std::unique_lock<std::mutex> lock(cout_mutex);
        std::cout << "smoker with matches\n";
    }
}

void smoker_tobacco(void)
{
    for (;;) {
        tobacco_sem.wait();
        agent_sem.signal();

        std::unique_lock<std::mutex> lock(cout_mutex);
        std::cout << "smoker with tobacco\n";
    }
}

void smoker_paper(void)
{
    for (;;) {
        paper_sem.wait();
        agent_sem.signal();

        std::unique_lock<std::mutex> lock(cout_mutex);
        std::cout << "smoker with paper\n";
    }
}

int main(void)
{
    std::vector<std::thread> threads;
    threads.emplace_back(agent_a);
    threads.emplace_back(agent_b);
    threads.emplace_back(agent_c);
    threads.emplace_back(smoker_matches);
    threads.emplace_back(smoker_tobacco);
    threads.emplace_back(smoker_paper);
    threads.emplace_back(pusher_tobacco);
    threads.emplace_back(pusher_match);
    threads.emplace_back(pusher_paper);

    for (auto &thread : threads) {
        thread.join();
    }
    return 0;
}
