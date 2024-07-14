#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class LRUCache {
private:
    int capacity;
    int numSets;
    vector<vector<pair<int, int>>> cache;

public:
    LRUCache(int capacity, int numSets) {
        this->capacity = capacity / numSets;
        this->numSets = numSets;
        for (int i = 0; i < capacity; i++) {
            vector<pair<int, int>> set;
            for (int j = 0; j < numSets; j++) {
                pair<int, int> p = make_pair(-1, -1);
                set.push_back(p);
            }
            cache.push_back(set);
        }
    }

    void print() {
        for (int i = 0; i< capacity; i++) {
            for (int j = 0; j < numSets; j++) {
                cout << cache[i][j].first << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool find(int val, int index) {
        bool found = false;

        for (int i = 0; i< capacity; i++) {
            for (int j = 0; j < numSets; j++) {
                if (cache[i][j].first == val) {
                    found = true;
                    cache[i][j].second++;
                    break;
                }
            }
        }

        return found;
    }

    void replace(int val, int index) {
        int address = index % capacity;
        int minTime = index;
        int minIndex;
        for (int j = 0; j < numSets; j++) {
            if (cache[address][j].second < minTime) {
                minTime = cache[address][j].second;
                minIndex = j;
            }
        }
        cache[address][minIndex] = make_pair(val, 1);
        return;
    }
};

int main() {
    int cacheSize, numSets, dataSize;
    cin >> cacheSize >> numSets >> dataSize;

    LRUCache cache(cacheSize, numSets);
    int data;
    int cacheMisses = 0;

    for (int i = 0; i < dataSize; i++) {
        cin >> data;
        // cache.print();
        if (!cache.find(data, i)) {
            cache.replace(data, i);
            cacheMisses++;
        }
    }

    cout << "Total cache misses:" << cacheMisses << endl;

    return 0;
}
