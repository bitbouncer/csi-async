#include <thread>
#include <chrono>
#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <boost/asio.hpp>
#include <csi-async/async.h>

void async_sleep(boost::asio::io_service& ios, int64_t ms, std::function<void(int ec)> cb) {
  auto h = std::make_shared<boost::asio::deadline_timer>(ios);
  h->expires_from_now(boost::posix_time::milliseconds(ms));
  std::cout << "sleeping for " << " (time : " << ms << " ms)" << std::endl;
  h->async_wait([h, ms, cb](const boost::system::error_code& ec) {
    std::cout << "sleep is over : " << " (time : " << ms << " ms)" << std::endl;
    cb(!ec ? 0 : -1);
  });
}

int main() {
  using namespace std::chrono_literals;
  boost::asio::io_service io_service;
  std::unique_ptr<boost::asio::io_service::work> nullwork = std::make_unique<boost::asio::io_service::work>(io_service);
  std::thread background_thread([&] { io_service.run(); });

  auto outer(std::make_shared<csi::async::work<int>>(csi::async::SEQUENTIAL, csi::async::FIRST_FAIL));

  for (auto steps : {1,2,3}) {

    auto inner(std::make_shared<csi::async::work<int>>(csi::async::PARALLEL, csi::async::FIRST_SUCCESS)); // this will be each 100ms delay
    for (auto delay : {5,4,3,2,1}) {
      inner->push_back([&io_service, delay](csi::async::work<int>::callback cb) {
        async_sleep(io_service, delay * 100, cb);
      });
    }
    outer->push_back(*inner);
  }

  std::cout << "start ASYNC" << std::endl;
  auto start_ts = std::chrono::steady_clock::now();
  auto res = (*outer)();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_ts);
  std::cout << "done ASYNC duration: " << duration.count() << " ms" << std::endl;

  std::cout << "resetting dummy work" << std::endl;
  nullwork.reset();
  background_thread.join();
  std::cout << "exiting" << std::endl;
  return 0;
}