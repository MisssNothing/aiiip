#include <iostream>

struct p_t {
  int x, y;
};
struct f_t {
  p_t aa, bb;
};
struct IVertex {
  void vertexes(p_t** res, size_t& s) const
  {
    p_t * p = new p_t[s + size()];
    for (size_t i = 0; i < s; ++i) {
      p[i] = (*res)[i];
    }
    for (size_t i = 0; i < size(); ++i) {
      p[i + s] = vertex(i);
    }
    delete[] (*res);
    *res = p;
    s += size();
  }
  protected:
    ~IVertex() = default;
  private:
    virtual size_t size() const = 0;
    virtual p_t vertex(size_t i) const = 0;
};
struct IFrame: IVertex {
  virtual ~IFrame() = default;
  f_t frame() const;
  private:
    virtual p_t aa() const = 0;
    virtual p_t bb() const = 0;
};
struct Segment final: IFrame {
  Segment(p_t a, p_t b): a_(a), b_(b) {}
  size_t size() const override;
  p_t vertex(size_t i) const override;
  p_t aa() const override;
  p_t bb() const override;
  private:
    p_t a_, b_;
};
p_t Segment::aa() {
  int minx = a_.x < b_.x ? a_.x : b_.x;
  int miny = a_.y < b_.y ? a_.y : b_.y;
  return p_t{ minx, miny };
}
p_t Segment::bb() {
  int maxx = a_.x > b_.x ? a_.x : b_.x;
  int maxy = a_.y > b_.y ? a_.y : b_.y;
  return p_t{ maxx, maxy };
}
size_t Segment::size() {
  return 2;
}
p_t IVertex::vertex(size_t i) {
  return ;
}
