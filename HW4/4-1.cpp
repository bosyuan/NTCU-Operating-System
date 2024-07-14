#include <iostream>
#include <vector>
using namespace std;

int simulateDirectMapCache(int cacheSize, int dataSize, std::vector<int>& data) {
    vector<int> cache(cacheSize, -1); // Initialize cache with -1 (indicating empty)

    int misses = 0;

    for (int i = 0; i < dataSize; i++) {
        int address = i % cacheSize; // Calculate the index for direct mapping
        bool found = false;
        for (int j = 0; j < cacheSize; j++) {
            if (cache[j] == data[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            // cout << "Miss at address " << address  << " " << cache[address] << " " << data[i] << endl;
            cache[address] = data[i]; // Update cache with new data
            misses++; // Increment the miss count
        }
    }

    return misses;
}

int main() {
    int cacheSize, dataSize;
    cin >> cacheSize >> dataSize;

    vector<int> data(dataSize);
    for (int i = 0; i < dataSize; i++) {
        std::cin >> data[i];
    }

    int misses = simulateDirectMapCache(cacheSize, dataSize, data);
    cout << "Total Cache Misses:" << misses << endl;

    return 0;
}
