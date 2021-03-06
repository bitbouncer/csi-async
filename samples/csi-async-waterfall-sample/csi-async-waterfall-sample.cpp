#include <thread>
#include <chrono>
#include <iostream>       
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
  boost::asio::io_service io_service;
  std::unique_ptr<boost::asio::io_service::work> keepalive_work = std::make_unique<boost::asio::io_service::work>(io_service);
  std::thread asio_thread([&] { io_service.run(); });
  auto work = std::make_shared<csi::async::work<int>>(csi::async::SEQUENTIAL, csi::async::FIRST_FAIL);
  for (auto delay : {5,4,3,2,1}) {
    work->push_back([&io_service, delay](csi::async::work<int>::callback cb) {
      async_sleep(io_service, delay * 1000, cb);
    });
  }

  work->async_call([work](int64_t duration, int ec) {
    std::cout << "SEQUENTIAL work done ec: " << ec << " (time : " <<  " ms)" << std::endl;
  });

  keepalive_work.reset();
  asio_thread.join();
  std::cout << "exiting" << std::endl;
  return 0;
}