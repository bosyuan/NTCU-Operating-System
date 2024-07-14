#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int n, global_count = 0;
int num_threads = 1; // Default number of threads
mutex m;
// Function to count primes in a specific range [start, end)
void count_primes_range(int start, int end) {
    for (int i = start; i <= end; i++) {
        bool flag = false;
        if (i == 1) {
            continue;
        }
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            m.lock();
            global_count++;
            m.unlock();
        }
    }
}

int main(int argc, char *argv[]) {
    bool goodParam = false;
    for (int i=1; i<argc-1; i++) {
        if (string(argv[i]) == "-t") {
            goodParam = true;
            num_threads = stoi(argv[i+1]);
            cin >> n;
            break;
        }
    }
    if (!goodParam) {
        cout << "Usage: " << argv[0] << " -t <num_threads>" << endl;
        return 1;        
    }

    int range_size = n / num_threads;
    int remaining = n % num_threads;

    int start = 1;
    int end = start + range_size - 1;

    // Create threads and count primes in parallel
    thread threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        if (remaining > 0) {
            end++;
            remaining--;
        }

        threads[i] = thread(count_primes_range, start, end);

        start = end + 1;
        end = start + range_size - 1;
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    cout << global_count << endl;
    return 0;
}
