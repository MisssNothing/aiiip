#include <iostream>
namespace eee {
  struct p_t {
    int x, y;
  };
  bool operator==(p_t a, p_t b);
  bool operator!=(p_t a, p_t b);
}
int main()
{
  using namespace eee;
  p_t a{1, 1}, b{0, 1};
  std::cout << (a == b) << '\n';
}
bool eee::operator==(p_t a, p_t b) {
  return a.x == b.x && a.y == b.y;
}
bool eee::operator!=(p_t a, p_t b) {
  return !(a == b);
}
