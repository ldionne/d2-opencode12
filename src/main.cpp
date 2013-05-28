#define D2_ENABLED

#include "synchronized.hpp"
#include <chrono>
#include <d2/standard_thread.hpp>
#include <iostream>
#include <thread>


typedef d2::standard_thread<std::thread> Thread;

synchronized<int> x(10);
synchronized<int> y(5);

int main() {
    Thread t1([&] {
        // By changing the sleep duration, we can influence the likelihood of
        // triggering the deadlock.
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
        x += y;
    });

    Thread t2([&] {
        y += x;
    });

    t1.join();
    t2.join();

    std::cout << "x: " << x << "\ny: " << y << std::endl;
}
