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

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

enum class Solution {
    deadlock,
    rightie,
    limit_four
};

Semaphore seat_available(4);
#ifdef SOLN_RIGHTIE
constexpr auto solution = Solution::rightie;
#elif SOLN_LIMIT
constexpr auto solution = Solution::limit;
#else
constexpr auto solution = Solution::deadlock;
#endif


void philosopher(int id, std::shared_ptr<std::vector<std::unique_ptr<Semaphore>>> forks)
{
    const int n_phil = forks->size();
    const int left = id;
    const int right = (id + 1)%n_phil;

    for (;;) {
        if constexpr (solution == Solution::deadlock) {
            /* THINK */
            (*forks)[left]->wait();
            (*forks)[right]->wait();
            /* EAT */
            (*forks)[left]->signal();
            (*forks)[right]->signal();
        } else if constexpr (solution == Solution::rightie) {
            /* THINK */
            if (id == 0) {
                (*forks)[right]->wait();
                (*forks)[left]->wait();
            } else {
                (*forks)[left]->wait();
                (*forks)[right]->wait();
            }
            /* EAT */
            (*forks)[left]->signal();
            (*forks)[right]->signal();
        } else if constexpr (solution == Solution::limit_four) {
            /* THINK */
            seat_available.wait();
            (*forks)[left]->wait();
            (*forks)[right]->wait();
            /* EAT */
            (*forks)[left]->signal();
            (*forks)[right]->signal();
            seat_available.signal();
        }
    }
}

int main(void)
{
    constexpr auto n_phil = 5;

    auto forks = std::make_shared<std::vector<std::unique_ptr<Semaphore>>>();
    for (int i = 0; i < n_phil; ++i) {
        forks->push_back(std::make_unique<Semaphore>(1));
    }

    std::vector<std::thread> phils;
    for (int i = 0; i < n_phil; ++i) {
        phils.emplace_back(philosopher, i, forks);
    }

    for (int i = 0; i < n_phil; ++i) {
        phils[i].join();
    }
    return 0;
}
