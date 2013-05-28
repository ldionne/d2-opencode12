
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
        // triggering the deadlock, but not the correctness of the program.
        using namespace std::chrono;
        d2::core::is_enabled() ? std::this_thread::sleep_for(milliseconds(100))
                               : std::this_thread::sleep_for(nanoseconds(10));
        x += y;
    });

    Thread t2([&] {
        y += x;
    });

    t1.join();
    t2.join();

    std::cout << "x: " << x << "\ny: " << y << std::endl;
}
