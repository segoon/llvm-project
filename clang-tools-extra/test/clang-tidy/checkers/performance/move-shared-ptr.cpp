// RUN: %check_clang_tidy %s performance-move-shared-ptr %t

namespace std {
template<typename T>
class shared_ptr {
public:
  T& operator*() { return reinterpret_cast<T&>(*this); }
shared_ptr() {}
shared_ptr(const shared_ptr<T>&) {}
private:
};

template<typename T>
T&& move(T&)
{
}

} // namespace std

void f(std::shared_ptr<int>);

void f_arg(std::shared_ptr<int> ptr)
{
  if (*ptr)
    f(ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: Could be std::move() [performance-move-shared-ptr]
}

void f_rvalue_ref(std::shared_ptr<int>&& ptr)
{
  if (*ptr)
    f(ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: Could be std::move() [performance-move-shared-ptr]
}

using SharedPtr = std::shared_ptr<int>;
void f_using(SharedPtr ptr)
{
  if (*ptr)
    f(ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: Could be std::move() [performance-move-shared-ptr]
}

void f_local()
{
  std::shared_ptr<int> ptr;
  if (*ptr)
    f(ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: Could be std::move() [performance-move-shared-ptr]
}

void f_move()
{
  std::shared_ptr<int> ptr;
  if (*ptr)
    f(std::move(ptr));
}

void f_ref(std::shared_ptr<int> &ptr)
{
  if (*ptr)
    f(ptr);
}

std::shared_ptr<int> f_return()
{
  std::shared_ptr<int> ptr;
  return ptr;
}

void f_still_used(std::shared_ptr<int> ptr)
{
  if (*ptr)
    f(ptr);

  *ptr = 1;
  *ptr = *ptr;
}
