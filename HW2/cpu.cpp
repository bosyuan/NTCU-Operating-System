#include <bits/stdc++.h>

using namespace std;

void write_result(vector<pair<int, int>>& result, const vector<pair<int, int>> ProcessQueue, int arrival, int finish) {
    for (int i=0; i<ProcessQueue.size(); i++) {
        if (ProcessQueue[i].first == arrival) {
            result[i] = make_pair(finish - arrival - ProcessQueue[i].second, finish - arrival);
            break;
        }
    }
    return;
}

int main (){
    int N, M, qType, tQunatum, arrival, bust;
    cin >> N >> M;
    vector<pair<int, int>> CPUINFO;
    vector<priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>>  CPU;
    vector<pair<int, int>> ProcessQueue;
    vector<pair<int, int>> result;

    //save input N, M
    for (int i=0; i<N; i++) {
        cin >> qType >> tQunatum;
        CPUINFO.push_back(make_pair(qType, tQunatum));
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        CPU.push_back(pq);
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
        cout <<"Current CPU TIME: " << CPUTime << " Incomming process: " << curr_process << endl;

        //push process into top queue when [arrival time = CPU time]
        if (curr_process < M) {
            pair<int, int> ProcessIncome;
            ProcessIncome = ProcessQueue[curr_process];
            if (ProcessIncome.first == CPUTime) {
                qType = CPUINFO[0].first;
                if (qType == 0) {
                    //FCFS
                    CPU[0].push(ProcessIncome);
                }
                else if (qType == 1) {
                    //SRTF
                    //  if push doesn't cause interrupt
                    if (curr_CPU != 0) {
                        CPU[0].push(make_pair(ProcessIncome.second, ProcessIncome.first));
                    }
                    //  push may causes interrupt -> place front process to queue 1
                    else {
                        pair<int, int> prev_proc = CPU[0].top();
                        CPU[0].pop();
                        CPU[0].push(make_pair(ProcessIncome.second, ProcessIncome.first));
                        if (prev_proc.first > ProcessIncome.second && N > 1) {
                            if (CPUINFO[1].first == 1) CPU[1].push(prev_proc);
                            else CPU[1].push(make_pair(prev_proc.second, prev_proc.first));
                        }
                        else {
                            CPU[0].push(prev_proc);
                        }
                    }

                }
                else if (qType == 2) {
                    //RR
                }
                curr_process++;
            }
        }
        for (int i=0; i<N; i++) {
            if (!CPU[i].empty()) {

                // cheeck for preemtive process
                if (curr_CPU != -1 && curr_CPU != i) {
                    //pop the terminated process from its queue
                    int c = CPUINFO[curr_CPU].first;
                    int n = CPUINFO[curr_CPU].first;
                    pair<int, int> front = CPU[curr_CPU].top();
                    CPU[curr_CPU].pop();
                    if (curr_CPU+1 < N) {
                        if (c != 1 && n == 1 || c == 1 && n != 1) CPU[curr_CPU+1].push(make_pair(front.second, front.first));
                        else CPU[curr_CPU+1].push(front);
                    }
                    else {
                        CPU[curr_CPU].push(front);
                    }
                }
                
                // run 1 cycle of each queue
                qType = CPUINFO[i].first;
                if (qType == 0) {
                    //FCFS
                    pair<int, int> proc = CPU[i].top();
                    proc.second -= 1;
                    CPU[i].pop();
                    if (proc.second > 0) { 
                        CPU[i].push(proc);
                        curr_CPU = i;
                    }
                    else {
                        write_result(result, ProcessQueue, proc.first, CPUTime+1);
                        curr_CPU = -1;
                    }
                }
                else if (qType == 1) {
                    //SRTF
                    pair<int, int> proc = CPU[i].top();
                    proc.first -= 1;
                    CPU[i].pop();
                    if (proc.first > 0) { 
                        CPU[i].push(proc);
                        curr_CPU = i;
                    }
                    else {
                        for (int j=0; j<ProcessQueue.size(); j++) {
                            //cout << "finish process " << ProcessQueue[j].first << " " << proc.second << endl;
                            if (ProcessQueue[j].first == proc.second) {
                                result[j] = make_pair(CPUTime+1 - proc.second - ProcessQueue[j].second, CPUTime+1 - proc.second);
                                break;
                            }
                        }
                        curr_CPU = -1;
                    }
                }
                else if (qType == 2) {
                    //RR
                }
                break;
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
        cout << result[i].first << " " << result[i].second << endl;
        wait_sum += result[i].first;
        turn_sum += result[i].second;
    }

    cout << wait_sum << endl;
    cout << turn_sum << endl;
    return 0;
}