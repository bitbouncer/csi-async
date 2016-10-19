#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include <csi-async/spinlock.h>

csi::spinlock _lock;
volatile int count = 0;

void f1(int n) {
  for (int cnt = 0; cnt < 100; ++cnt) {
    _lock.lock();
    count++;
    std::cout << "Output from thread " << n << '\n';
    _lock.unlock();
  }
}

void f2(int n) {
  for (int cnt = 0; cnt < 100; ++cnt) {
    csi::spinlock::scoped_lock xx(_lock);
    count++;
    std::cout << "Output from thread " << n << '\n';
  }
}

void f3(int n) {
  for (int cnt = 0; cnt < 100; ++cnt) {
    std::unique_lock<csi::spinlock> xx(_lock);
    count++;
    std::cout << "Output from thread " << n << '\n';
  }
}

void f4(int n) {
  for (int cnt = 0; cnt < 100; ++cnt) {
    std::lock_guard<csi::spinlock> xx(_lock);
    count++;
    std::cout << "Output from thread " << n << '\n';
  }
}


int main() {
  std::vector<std::thread> v;
  for (int n = 0; n < 10; ++n) {
    v.emplace_back(f4, n);
  }
  for (auto& t : v) {
    t.join();
  }
  std::cout << count << std::endl;
}
