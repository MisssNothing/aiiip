#include <iostream>

struct IDraw {
  virtual p_t begin() const = 0;
  virtual p_t next(p_t p) const = 0;
  virtual aIDraw() = default;
};

