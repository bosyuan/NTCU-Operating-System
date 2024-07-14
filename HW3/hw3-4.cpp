#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <mutex>
#include <thread>

using namespace std;

int n, m;
int num_threads = 1;
mutex write_lock;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

void solve(int index, uint64_t current) {
    if (index == m) {
        if (current == (one << n) - 1) {
            global_count++;
        }
    } else {
        solve(index + 1, current);
        solve(index + 1, current | subsets[index]);
    }
}

void thread_solve(uint64_t &local_count, int index, uint64_t current) {
    if (index == m) {
        if (current == one) {
            local_count++;
        }
    } else {
        thread_solve(local_count, index + 1, current);
        thread_solve(local_count, index + 1, current | subsets[index]);
    }
}

void task(int index, uint64_t current) {
    uint64_t local_count = 0;
    thread_solve(local_count, index, current);
    global_count += local_count;
}

int main(int argc, char *argv[]) {

    bool goodParam = false;
    for (int i=1; i<argc-1; i++) {
        if (string(argv[i]) == "-t") {
            goodParam = true;
            num_threads = stoi(argv[i+1]);
            break;
        }
    }
    if (!goodParam) {
        cout << "Usage: " << argv[0] << " -t <num_threads>" << endl;
        return 1;        
    }

    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    thread threads[num_threads];
    int k = log2(num_threads);
    if (k == 1 || k >= m)
        solve(0, 0);
    else{
        one = (one << n) - 1;
        for (int i = 0; i < num_threads; i++) {
            int remain = i;
            uint64_t current = 0;
            for (int j = 0; j < k; j++) {
                int add2subset = remain % 2;
                remain /= 2;
                if (add2subset == 1) {
                    current = current | subsets[j];
                }
            }
            threads[i] = thread(task, k, current);
        }

        for (int i = 0; i < num_threads; i++) {
            threads[i].join();
        }
    }

    cout << global_count << endl;
    return 0;
}
