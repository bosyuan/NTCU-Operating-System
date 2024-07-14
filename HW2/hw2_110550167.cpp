#include <bits/stdc++.h>

using namespace std;

class Q
{
public:
    virtual void run(){};
    Q(/* args */){};
    ~Q(){};
};

class FIFO_Q : public Q
{
public:
    struct cmp
    {
        bool operator()(vector<int> a, vector<int> b) {
            return a[0] > b[0];
        }
    };
    priority_queue<vector<int>, vector<vector<int>>, cmp> queue;

    void run(){};
    FIFO_Q(/* args */){};
};

class SRTF_Q : public Q
{
public:
    struct cmp
    {
        bool operator()(vector<int> a, vector<int> b) {
            if (a[2] == b[2]) {
                return a[1] > b[1];
            }
            return a[2] > b[2];
        }
    };
    priority_queue<vector<int>, vector<vector<int>>, cmp> queue;

    void run(){};
    SRTF_Q(/* args */){};
};

class RR_Q : public Q
{
public:
    struct cmp
    {
        bool operator()(vector<int> a, vector<int> b) {
            if (a[0] == b[0]) return a[1] < b[1];
            return a[0] > b[0];
        }
    };
    priority_queue<vector<int>, vector<vector<int>>, cmp> queue;
    int timer;
    void run(){};
    RR_Q(/* args */){ timer = 0;};
};

void write_result(vector<pair<int, int>>& result, const vector<pair<int, int>> ProcessQueue, int arrival, int finish) {
    for (int i=0; i<ProcessQueue.size(); i++) {
        if (ProcessQueue[i].first == arrival) {
            result[i] = make_pair(finish - arrival - ProcessQueue[i].second, finish - arrival);
            break;
        }
    }
    return;
}

void pushQueue(Q* parent, int type, vector<int> process) {
    if (type == 0){
        FIFO_Q* fifo = dynamic_cast<FIFO_Q*>(parent);
        fifo->queue.push(process);
    }
    else if (type == 1) {
        SRTF_Q* srtf = dynamic_cast<SRTF_Q*>(parent);
        srtf->queue.push(process);
    }
    else {
        RR_Q* rr = dynamic_cast<RR_Q*>(parent);
        rr->queue.push(process);
    }
} 

vector<int> popQueue(Q* parent, int type) {
    vector<int> front;
    if (type == 0){
        FIFO_Q* fifo = dynamic_cast<FIFO_Q*>(parent);
        front = fifo->queue.top();
        fifo->queue.pop();
    }
    else if (type == 1) {
        SRTF_Q* srtf = dynamic_cast<SRTF_Q*>(parent);
        front = srtf->queue.top();
        srtf->queue.pop();
    }
    else {
        RR_Q* rr = dynamic_cast<RR_Q*>(parent);
        front = rr->queue.top();
        rr->queue.pop();
        rr->timer = 0;
    }
    return front;
}

int main (){
    int N, M, qType, tQunatum, arrival, bust;
    cin >> N >> M;
    vector<pair<int, int>> CPUINFO;
    vector<Q*>  CPU;
    vector<pair<int, int>> ProcessQueue;
    vector<pair<int, int>> result;

    //save input N, M
    for (int i=0; i<N; i++) {
        cin >> qType >> tQunatum;
        CPUINFO.push_back(make_pair(qType, tQunatum));
        Q* qptr;
        if (qType == 0) qptr = new FIFO_Q();
        else if (qType == 1) qptr = new SRTF_Q();
        else if(qType == 2) qptr = new RR_Q();
        CPU.push_back(qptr);
    }

    //save input process to ProcessQueue and initialize result array
    for (int i=0; i<M; i++) {
        cin >> arrival >> bust;
        ProcessQueue.push_back(make_pair(arrival, bust));
        result.push_back(make_pair(-1, -1));
    }

    int CPUTime = 0;
    int curr_process = 0;
    int curr_CPU = -1;
    bool finish = false;
    
    //sequential CPU runner
    while(true) {
        // std::cout <<"Current CPU TIME: " << CPUTime << " In process CPU: " << curr_CPU << endl;

        //push process into top queue when [arrival time = CPU time]
        if (curr_process < M) {
            pair<int, int> ProcessIncome;
            ProcessIncome = ProcessQueue[curr_process];
            if (ProcessIncome.first == CPUTime) {
                qType = CPUINFO[0].first;
                Q* qptr = CPU[0];
                vector<int> vect{CPUTime, ProcessIncome.first, ProcessIncome.second};
                if (qType == 0) {
                    //FCFS
                    FIFO_Q* childptr = dynamic_cast<FIFO_Q*>(qptr);
                    childptr->queue.push(vect);
                }
                else if (qType == 1) {
                    //SRTF
                    //  if push doesn't cause interrupt
                    SRTF_Q* childptr = dynamic_cast<SRTF_Q*>(qptr);
                    if (curr_CPU != 0) {
                        childptr->queue.push(vect);
                    }
                    //  push may causes interrupt -> place front process to queue 1
                    else {
                        vector<int> prev = childptr->queue.top();
                        childptr->queue.pop();
                        childptr->queue.push(vect);
                        if (prev[2] > ProcessIncome.second) {
                            Q* nextQueue = CPU[1];
                            prev[0] = CPUTime;
                            if (N > 1) {
                                pushQueue(nextQueue, CPUINFO[1].first, prev);
                            }
                            else {
                                childptr->queue.push(prev);
                            }
                        }
                        else {
                            childptr->queue.push(prev);
                        }
                    }

                }
                else if (qType == 2) {
                    RR_Q* rr = dynamic_cast<RR_Q*>(qptr);
                    rr->queue.push(vect);
                }
                curr_process++;
            }
        }
        for (int i=0; i<N; i++) {

            Q* nextQueue = CPU[i];
            if (CPUINFO[i].first == 0){
                FIFO_Q* fifo = dynamic_cast<FIFO_Q*>(nextQueue);
                if (!fifo->queue.empty()) {
                    if (curr_CPU != -1 && curr_CPU != i) {
                        vector<int> front = popQueue(CPU[curr_CPU], CPUINFO[curr_CPU].first);
                        if (curr_CPU+1 >= N) {
                            front[0] = CPUTime;
                            pushQueue(CPU[curr_CPU], CPUINFO[curr_CPU].first, front);
                        }
                        else {
                            front[0] = CPUTime;
                            pushQueue(CPU[curr_CPU+1], CPUINFO[curr_CPU+1].first, front);
                        }
                    }
                    vector<int> front = fifo->queue.top();
                    front[2] -= 1;
                    fifo->queue.pop();
                    if (front[2] > 0) {
                        fifo->queue.push(front);
                        curr_CPU = i;
                    }
                    else {
                        write_result(result, ProcessQueue, front[1], CPUTime+1);
                        curr_CPU = -1;
                    }
                    break;
                }
            }
            else if (CPUINFO[i].first == 1) {
                SRTF_Q* srtf = dynamic_cast<SRTF_Q*>(nextQueue);
                if (!srtf->queue.empty()) {
                    if (curr_CPU != -1 && curr_CPU != i) {
                        vector<int> front = popQueue(CPU[curr_CPU], CPUINFO[curr_CPU].first);
                        if (curr_CPU+1 >= N) {
                            front[0] = CPUTime;
                            pushQueue(CPU[curr_CPU], CPUINFO[curr_CPU].first, front);
                        }
                        else {
                            front[0] = CPUTime;
                            pushQueue(CPU[curr_CPU+1], CPUINFO[curr_CPU+1].first, front);
                        }
                    }
                    vector<int> front = srtf->queue.top();
                    front[2] -= 1;
                    srtf->queue.pop();
                    if (front[2] > 0) {
                        srtf->queue.push(front);
                        curr_CPU = i;
                    }
                    else {
                        write_result(result, ProcessQueue, front[1], CPUTime+1);
                        curr_CPU = -1;
                    }
                    break;
                }
            }
            else if (CPUINFO[i].first == 2){
                RR_Q* rr = dynamic_cast<RR_Q*>(nextQueue);
                //cout << "process in Queue " << rr->queue.size() << endl; 
                if (!rr->queue.empty()) {
                    if (curr_CPU != -1 && curr_CPU != i) {
                        vector<int> front = popQueue(CPU[curr_CPU], CPUINFO[curr_CPU].first);
                        if (curr_CPU+1 >= N) {
                            front[0] = CPUTime;
                            pushQueue(CPU[curr_CPU], CPUINFO[curr_CPU].first, front);
                        }
                        else {
                            front[0] = CPUTime;
                            pushQueue(CPU[curr_CPU+1], CPUINFO[curr_CPU+1].first, front);
                        }
                    }
                    vector<int> front = rr->queue.top();
                    front[2] -= 1;
                    rr->timer++;
                    rr->queue.pop();
                    if (front[2] > 0) {
                        if (rr->timer < CPUINFO[i].second) {
                            rr->queue.push(front);
                            curr_CPU = i;
                        }
                        else {
                            rr->timer = 0;
                            curr_CPU = -1;
                            front[0] = CPUTime+1;
                            if (i+1 < N) pushQueue(CPU[i+1], CPUINFO[i+1].first, front);
                            else rr->queue.push(front);
                        }
                    }
                    else {
                        write_result(result, ProcessQueue, front[1], CPUTime+1);
                        curr_CPU = -1;
                        rr->timer = 0;
                    }
                    break;
                }
            }

            if (i+1 == N && curr_process >= M) {
                finish = true;
            }
        }
        CPUTime++;
        if (finish) break;
    }

    int wait_sum = 0, turn_sum = 0;
    for (int i=0; i<result.size(); i++) {
        std::cout << result[i].first << " " << result[i].second << endl;
        wait_sum += result[i].first;
        turn_sum += result[i].second;
    }

    std::cout << wait_sum << endl;
    std::cout << turn_sum << endl;
    return 0;
}