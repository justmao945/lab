#include <stdio.h>
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

template <class T> class Channel {
  std::mutex mutex;
  std::queue<T> queue;
  std::condition_variable not_full, not_empty;
  size_t capacity;

public:
  explicit Channel(size_t capacity = 1) : capacity(capacity) {}

  T read() {
    T t;
    std::unique_lock<std::mutex> ul(mutex);
    // lock and wait when empty
    //  wait means release lock and sleep thread
    //  wakeup and re-lock when notified
    not_empty.wait(ul, [this]() { return !queue.empty(); });
    t = queue.front();
    queue.pop();
    ul.unlock();
    not_full.notify_one();
    return t;
  }

  void write(const T &t) {
    std::unique_lock<std::mutex> ul(mutex);
    // wait when full
    not_full.wait(ul, [this]() { return capacity != queue.size(); });
    queue.push(t);
    ul.unlock();
    not_empty.notify_one();
  }
};

class Producer {
  Channel<int> &c;

public:
  explicit Producer(Channel<int> &c) : c(c) {}

  void operator()() {
    for (int i = 0; i < 10; i++) {
      printf("%ld: try to write %d\n", std::this_thread::get_id(), i);
      c.write(i);
      printf("%ld: write %d done\n", std::this_thread::get_id(), i);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
};

class Consumer {
  Channel<int> &c;

public:
  explicit Consumer(Channel<int> &c) : c(c) {}

  void operator()() {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    while (true) {
      printf("%ld: try to read\n", std::this_thread::get_id());
      int x = c.read();
      printf("%ld: read %d done\n", std::this_thread::get_id(), x);
    }
  }
};

int main() {
  Channel<int> c(2);
  Producer p(c);
  Consumer s(c);
  std::thread t1(p), t2(s), t3(p), t4(s);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  return 0;
}
