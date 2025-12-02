#include <iostream>
namespace eee {
  struct p_t {
    int x, y;
  };
  struct f_t {
    p_t aa, bb;
  };
  bool operator==(p_t a, p_t b);
  bool operator!=(p_t a, p_t b);
  struct IDraw {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t prev) const = 0;
    virtual ~IDraw() = default;
  };
  struct Dot: IDraw {
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t d;
  };
}
int main()
{
  using namespace eee;
  p_t a{1, 1}, b{0, 1};
  std::cout << (a == b) << '\n';
}
eee:p_t eee::Dot::begin() const {
  return d;
}
eee:p_t eee::Dot::next(p_t prev) const {
  if (prev != d) {
    throw std::logic_error("bad prev");
  }
  return d;
}
bool eee::operator==(p_t a, p_t b) {
  return a.x == b.x && a.y == b.y;
}
bool eee::operator!=(p_t a, p_t b) {
  return !(a == b);
}


