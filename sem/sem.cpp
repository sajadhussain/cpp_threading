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

/* Parts of the code is adapted from the example semaphore implementation
 * provided in The Little Book of Semaphores.
 */

#include "sem.h"

#include <mutex>

Semaphore::Semaphore(int val) :
    value(val)
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

bool Semaphore::trywait()
{
    std::unique_lock<std::mutex> lock(mutex);

    if (value > 0) {
        --value;
        return true;
    } else {
        return false;
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
