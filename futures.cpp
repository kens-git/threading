#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <future>
#include <functional>

using std::cout;
using std::endl;
using std::vector;
using std::find;
using std::begin;
using std::end;
using std::sqrt;

using std::thread;
using std::async;
using std::future;

using std::ref;

using HRC = std::chrono::high_resolution_clock;
using std::chrono::duration;

// generate nums between starting and end, inclusive
vector<unsigned int> generateNums(unsigned int starting, unsigned int end);
vector<unsigned int> generateNumsConcurrently(unsigned int starting, unsigned int end);
void printElapsedTime(duration<double> elapsed);

int main() {
    unsigned int numsStart = 0;
    unsigned int numsFinish = 10000000;

    HRC::time_point start;
    start = HRC::now();
    vector<unsigned int> singleNums = generateNums(numsStart, numsFinish);
    printElapsedTime(HRC::now() - start);

    // Because the vectors in each thread are merged in the end, generateNums actually
    // runs faster that generateNumsConcurrently. Fine for a simple futures example, but defeats
    // the purpose of concurrency in production. One way to avoid that would be to have
    // generateNumsConcurrently simply return the 'results' vector.
    start = HRC::now();
    vector<unsigned int> multiNums = generateNumsConcurrently(numsStart, numsFinish);
    printElapsedTime(HRC::now() - start);

    return 0;
}

void printElapsedTime(duration<double> elapsed) {
    cout << endl << elapsed.count() << 's' << endl << endl;
}

vector<unsigned int> generateNumsConcurrently(unsigned int starting, unsigned int end) {
    unsigned char maxNumThreads = thread::hardware_concurrency();

    vector<future<vector<unsigned int>>> results;

    for (int i = 0; i < maxNumThreads; i++) {
        unsigned int min = i * (end - starting) / maxNumThreads;
        unsigned int max = min + (end - starting) / maxNumThreads;
        // keeps the min and max from overlapping the other thread's range
        if (i < maxNumThreads - 1) {
            max--;
        }

        //cout << min << " " << max << endl;
        results.push_back(async(std::launch::async, generateNums, min, max));
    }

    vector<unsigned int> nums(end - starting);
    for (int i = 0; i < results.size(); i++) {
        // We're keeping a copy of the result from each thread, because a future can only have
        // get() called on it once (it will throw an exception after that). Using a shared_future<>
        // is one way to avoid that problem.
        vector<unsigned int> temp = results[i].get();

        nums.insert(nums.end(), temp.begin(), temp.end());
    }

    return nums;
}

vector<unsigned int> generateNums(unsigned int start, unsigned int end) {
    vector<unsigned int> nums(end - start);

    for (int i = start; i < end + 1; i++) {
        nums.push_back(i);
    }

    return nums;
}
