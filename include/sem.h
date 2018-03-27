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

#ifndef SEM_H
#define SEM_H

#include <condition_variable>
#include <mutex>

class Semaphore {
public:
    Semaphore(int val);
    void wait();
    /* Try to decrement,
     * return true on success,
     * return false on failure
     */
    bool trywait();
    void signal();

private:
    std::mutex mutex;
    std::condition_variable cond;
    int value;
    int wakeups = 0;
};

#endif
