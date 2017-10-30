#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::lock_guard;
using std::unique_lock;

mutex mut;
condition_variable data_cond;

void importantProcessing() {
    lock_guard<mutex> lk(mut);

    cout << "Processing important stuff..." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    data_cond.notify_one();
}

void wontWait() {
    mutex m;
    lock_guard<mutex> lk(m);
    cout << "Corrupted data...\n";
}

void willWait() {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk);
    cout << "...processed valid data." << endl;
}

int main() {
    thread t1(importantProcessing);
    thread t2(wontWait);
    thread t3(willWait);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
