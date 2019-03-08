#include "ThreadPool.h"

using std::thread;
using std::vector;
using std::mutex;
using std::lock_guard;

ThreadPool::ThreadPool(vector<thread>::size_type n) {
  // TODO Create vector in initialisation
  for (vector<thread>::size_type i(0); i < n; ++i)
    threads.push_back(thread(&ThreadPool::thread_loop, this));
}

void ThreadPool::add_job(ThreadPool::job j) {
  lock_guard<mutex> lock(jobs_mutex);
  jobs.push(j);
}

void ThreadPool::abort(void) {
  set_state(ThreadPool::State::Abort);
}

ThreadPool::State ThreadPool::get_state(void) {
  lock_guard<mutex> lock(state_mutex);
  return state;
}

void ThreadPool::set_state(ThreadPool::State st) {
  lock_guard<mutex> lock(state_mutex);
  state = st;
}

void ThreadPool::thread_loop(void) {
  // TODO Sleep when nothing to do
  job j;
  while (true) {
    ThreadPool::State st = get_state();
    if (st == ThreadPool::State::Abort)
      return;
    {
      lock_guard<mutex> lock(jobs_mutex);
      if (!jobs.empty()) {
        j = jobs.top();
        jobs.pop();
      }
      else if (st == ThreadPool::State::Terminate)
        return;
    }
    if (j) {
      j();
      j = nullptr;
    }
  }
}

ThreadPool::~ThreadPool(void) {
  set_state(ThreadPool::State::Terminate);
  for (auto& t : threads) {
    t.join();
  }
}
