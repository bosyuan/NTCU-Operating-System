#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>

using namespace std;
sem_t sem[100];

void worker(int index) {
  static mutex io_mutex;
  int num = 1000000;
  while (num--) {}
  {
    sem_wait(&sem[index]);
    lock_guard<mutex> lock(io_mutex);
    cout << "I'm thread " << index << ", local count: 1000000\n";
    sem_post(&sem[index+1]);
  }
}

int main(void) {
  thread t[100];
  
  for (int i=0; i < 100; i++)
    sem_init(&sem[i], 0, 0);
  sem_init(&sem[0], 0, 1);

  for (int i = 0; i < 100; i++)
    t[i] = thread(worker, i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
