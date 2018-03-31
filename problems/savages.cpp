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

/* The dining savages problem */

#include "sem.h"

#include <iostream>
#include <thread>
#include <vector>

constexpr int pot_capacity = 7;
constexpr int savages = 5;

auto pot_full = Semaphore(0);
auto pot_empty = Semaphore(0);
auto servings_mutex = Semaphore(1);
int servings = 0;

void savage()
{
    for (;;) {
        servings_mutex.wait();
        if (servings == 0) {
            /* Pot is empty.
             * Notify the cook, wait until the pot is full again.
             */
            pot_empty.signal();
            pot_full.wait();
        }
        /* Pot is not empty, get servings and eat.
         */
        --servings;
        servings_mutex.signal();
    }
}

void cook()
{
    for (;;) {
        pot_empty.wait();
        /* Pot is empty, put servings */
        servings = pot_capacity;
        pot_full.signal();
    }
}

int main()
{
    std::thread cook_thread(cook);
    std::vector<std::thread> savage_threads;

    for (int i = 0; i < savages; ++i) {
        savage_threads.emplace_back(savage);
    }

    cook_thread.join();
    for (auto &savage_thread : savage_threads) {
        savage_thread.join();
    }

    return 0;
}
