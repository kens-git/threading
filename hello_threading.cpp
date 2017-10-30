#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::ref;
using std::unique_ptr;
using std::make_unique;
using std::move;

class Object {
    public:
        Object(int num) { mNum = num; }
        void multiplyNum(int num) { cout << mNum * num << endl; }

    private:
        int mNum;
};

void func(const string& message) {
    cout << "Received the message: ";
    cout << "\"" << message << "\"" << endl;
}

void pointerFunc(unique_ptr<string> message) {
    cout << *message << endl;
}

void func2() {
    for (int i = 0; i < 100000000; i++) {

    }
}

thread func3() {
    return thread(func2);
}

int main() {
    // thread() copies the string literal as-is, it is only converted to a std::string
    // in the context of the new thread.
    thread t1(func, "Passing string as parameter");
    t1.join();

    // Passing a string as an actual reference, because the default behaviour of thread()
    // is to copy the variables, even if the function takes a reference.
    string message = "the message";
    thread t2(func, ref(message));
    t2.join();

    // Passing member method to a new thread
    Object myObj(10);
    thread t3(&Object::multiplyNum, &myObj, 10);
    t3.join();

    // Moving objects into a thread
    unique_ptr<string> message2 = make_unique<string>("the message");
    thread t4(pointerFunc, move(message2));
    t4.join();

    // Moving threads
    thread t5(func2);
    thread t6(move(t5)); // t5 now has no associated thread
    t6.join();
    cout << "Finished t6" << endl;

    // Returning threads from a function
    thread t7 = func3();
    t7.join();
    cout << "Finished t7" << endl;

    // Polling the hardware capabilities regarding multithreading
    // trivia: running more threads than the hardware supports is called oversubscription
    cout << "Current hardware supports " << thread::hardware_concurrency() << " truly concurrent threads" << endl;
    thread t8(func2);
    cout << "t8's thread ID is: " << t8.get_id() << endl;
    t8.join();
    cout << "main thread's ID is: " << std::this_thread::get_id() << endl;

    return 0;
}
