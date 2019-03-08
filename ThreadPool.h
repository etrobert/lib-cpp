#pragma once

#include <vector>
#include <stack>
#include <thread>
#include <mutex>

class ThreadPool {
  using job = std::function<void(void)>;

  enum State { Running, Terminate, Abort };

  public:
    ThreadPool(std::vector<std::thread>::size_type n);
    ~ThreadPool(void);

    void add_job(job);

    void abort();

    State get_state(void);

  private:
    void thread_loop(void);

    void set_state(State st);

    std::vector<std::thread> threads;

    std::stack<job> jobs;
    std::mutex jobs_mutex;

    State state;
    std::mutex state_mutex;
};
