#include <iostream>
#include <thread>
#include <future>
#include <functional>
#include <chrono>

using std::cout;
using std::endl;

using std::thread;
using std::future;
using std::promise;
using std::this_thread::sleep_for;

using std::ref;

using std::chrono::seconds;

void waitForDone(future<bool>& isDone) {
    cout << "Waiting for isDone..." << endl;
    cout << "bool done is now: " << isDone.get() << endl;
}

int main() {
    promise<bool> done;
    future<bool> isDone = done.get_future();

    thread t1(waitForDone, ref(isDone));

    sleep_for(seconds(3));

    // isDone.get() waits for done to change values
    done.set_value(true);

    t1.join();

    return 0;
}
