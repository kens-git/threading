#include <iostream>
#include <thread>
#include <mutex>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::lock;
using std::adopt_lock;
using std::call_once;
using std::once_flag;

void mutexLock(string& str) {
    mutex mut;
    // We can lock a mutex in two ways, by manually calling lock and unlock, or by using a lock_guard,
    // which will automatically unlock the mutex when the lock_guard is destructed (in this case,
    // when it goes out of scope).

    // Everything after this lock_guard is unavailable to any other threads to any other threads
    // that may try to access it, until it's unlocked
    lock_guard<mutex> guard(mut);
    str += "string stuff";
    str += "more string";
    str += "even more string stuff";
}

class Object {
    public:
        Object(const string& data) : mData(data) { }

        void doProcessing() { mData += "processing...processing..."; }
        mutex mMutex;

    private:
        string mData;
};

void noDeadlock(Object& obj1, Object& obj2) {
    // lock the two objects
    lock(obj1.mMutex, obj2.mMutex);
    // pass responsibility for the locks to lock_guard
    lock_guard<mutex> lock1(obj1.mMutex, adopt_lock);
    lock_guard<mutex> lock2(obj2.mMutex, adopt_lock);
    obj1.doProcessing();
    obj2.doProcessing();
}

void initialize() {
    cout << "initializing..." << endl;
}

once_flag flag;
void func() {
    call_once(flag, initialize);
    cout << "func() called." << endl;
}

int main() {
    /* race condition:
     *      Anything where the outcome depends on the relative ordering of execution of operations
     *      on two or more threads.
     *
     * mutual exclusion (mutex):
     *      Allows a thread to edit data/a data structure without interference from other threads.
     *      The thread locks the mutex associated with some code then unlocks it when it's done.
     *      It's vitally important when designing programs that make use of mutual exclusion and locks
     *      that the locked code is encapsulated, because other threads with access to pointers or
     *      references that the mutex is protecting may be accessible to other threads, even with a mutex.
     *
     * deadlock:
     *      When two threads need to lock two of the same mutexes, and they've each locked one each, but
     *      need to lock the other to continue. The C++ standard library provides std::lock, which can
     *      lock two or more mutexes at once without the risk of deadlocks. Deadlocks can also occur when
     *      two threads call join() on each other, because they both wind up waiting for the other to finish.
    */

    string str = "Hello, world!";
    mutexLock(str);

    Object obj1("data1");
    Object obj2("data2");
    noDeadlock(obj1, obj2);
    noDeadlock(obj1, obj2);

    thread t1(func);
    t1.join();

    thread t2(func);
    t2.join();

    return 0;
}
