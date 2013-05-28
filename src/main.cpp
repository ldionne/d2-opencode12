
#include "synchronized.hpp"
#include <iostream>
#include <thread>


synchronized<int> x(10);
synchronized<int> y(5);

int main() {
    std::thread t1([&] {
        x += y;
    });

    std::thread t2([&] {
        y += x;
    });

    t1.join();
    t2.join();

    std::cout << "x: " << x << "\ny: " << y << std::endl;
}
