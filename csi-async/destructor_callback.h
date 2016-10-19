#include <functional>
#pragma once

namespace csi {
namespace async {
template <typename Value>
class destructor_callback
{
  public:
  destructor_callback(std::function <void(Value& val)>  callback) : _cb(callback) {}
  destructor_callback(const Value& initial_value, std::function <void(Value& val)>  callback) : _val(initial_value), _cb(callback) {}
  ~destructor_callback() { _cb(_val); }
  Value& value() { return _val; }
  const Value& value() const { return _val; }
  private:
  std::function <void(Value& ec)> _cb;
  Value _val;
};
} // namespace
}// namespace