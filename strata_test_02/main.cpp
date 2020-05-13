#include <iostream>
#include <chrono>
#include <thread>

void worker_func(int n)
{
    std::cout << "thread" << n << ": starting" << std::endl;
    int i = 0;
    while (i < 10)
    {
        std::cout << "thread" << n << ": running"<<" :: "<<i++<<std::endl;
        // sleep for 200ms
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "thread" << n << ": ended" << std::endl;
}

int main()
{
    std::cout << "main: starting all threads" << std::endl;

    std::thread n1 (&worker_func,1);
    std::thread n2 (&worker_func,2);
    std::thread n3 (&worker_func,3);

    n1.join();
    n2.join();
    n3.join();

    std::cout << "main: completed" << std::endl;

    return 0;
}
