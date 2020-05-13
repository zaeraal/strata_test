#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <queue>

int thread_num = 3;
std::mutex m;
std::queue<std::condition_variable*> cvs;

void worker_func(int n, std::condition_variable* cv)
{
    std::cout << "thread" << n << ": starting, waiting" << std::endl;

    while(true) {
        std::unique_lock<std::mutex> ul(m);
        cv->wait(ul);
        std::cout << "thread" << n << ": signal received, doing work ...." << std::endl;

        cvs.pop();
        cvs.emplace(cv);

        // sleep for 1-5s
        int randTime = rand() % 5 + 1;
        std::this_thread::sleep_for(std::chrono::seconds(randTime));
        std::cout << "thread" << n << ": done with work, signal next thread" << std::endl;

        ul.unlock();
        if(!cvs.empty())
            cvs.front()->notify_one();
    }
    //std::cout << "thread" << n << ": ended" << std::endl;
}

struct sThreadHandler {
public:
    sThreadHandler(int n): m_thread(worker_func, n, &m_cv) { cvs.emplace(&m_cv); }
public:
    std::condition_variable m_cv;
    std::thread             m_thread;
};

int main()
{
    std::cout << "main: starting all threads" << std::endl;

    std::vector<sThreadHandler*> workers;

    for(int i = 1; i <= thread_num; ++i) {
        sThreadHandler* worker = new sThreadHandler(i);
        workers.push_back(worker);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));    // wait for all the threads

    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << "main: starting thread 1" << std::endl;
    }

    if(!cvs.empty())
        cvs.front()->notify_one();

    for(auto it : workers) {
        it->m_thread.join();
    }

    std::cout << "main: completed" << std::endl;

    return 0;
}
