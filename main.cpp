#include <iostream>
#include <stdexcept>
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
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t d;
  };
  struct HLine: IDraw {
    explicit HLine(p_t hl);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t h;
  };
  struct Triangle: IDraw {
    explicit Triangle(p_t hr);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t e;
  };
  struct Rect: IDraw {
    Rect(p_t pos, int w, int h);
    Rect(p_t a, p_t b);
    virtual p_t aa() const = 0;
    p_t begin() const override;
    p_t next(p_t prev) const override;
    f_t rect;
  };
  p_t * extend(const p_t* pts, size_t s, p_t fill);
  void extend(p_t** pts, size_t& s, p_t fill);
  void append(const IDraw* sh, p_t** ppts, size_t& s);
  f_t frame(const p_t * pts, size_t s);
  char * canvas(f_t fr, char fill);
  void paint(p_t p, char * cnv, f_t fr, char fill);
  void flush(std::ostream& os, const char* cnv, f_t fr);
  size_t rows(f_t fr);
  size_t cols(f_t fr);
}
int main()
{
  using namespace eee;
  int err = 0;
  int size_trian = 5;
  int other_trian = 0;
  int other = size_trian - 1;
  while (other != 0) {
    other_trian += other;
    --other;
  }
  int x = 0, y = 0;
  IDraw* shp[size_trian * size_trian - other_trian] = {};
  p_t * pts = nullptr;
  size_t s = 0;
  try {
    int j_trian = size_trian + 0;
    int shp_i = 0;
    for (int i = 0; i < size_trian; ++i) {
      for (int j = 0; j < j_trian; ++j) {
        shp[shp_i] = new Triangle({x + j, y + i});
        ++shp_i;
      }
      --j_trian;
    }
    for (int i = 0; i < size_trian * size_trian - other_trian; ++i) {
      append(shp[i], &pts, s);
    }
    f_t fr = frame(pts, s);
    char * cnv = canvas(fr, '.');
    for (size_t i = 0; i < s; ++i) {
      paint(pts[i], cnv, fr, '#');
    }
    flush(std::cout, cnv, fr);
    delete [] cnv;
  } catch (...) {
    std::cerr << "Error\n";
    err = 1;
  }
  for (int i = 0; i < size_trian * size_trian - other_trian; ++i) {
    delete shp[i];
  }
  return err;
}



eee::p_t * eee::extend(const p_t* pts, size_t s, p_t fill)
{
  p_t* r = new p_t[s + 1];
  for (size_t i = 0; i < s; ++i) {
    r[i] = pts[i];
  }
  r[s] = fill;
  return r;
}
void eee::extend(p_t** pts, size_t& s, p_t fill)
{
  p_t* r = extend(*pts, s, fill);
  delete[] *pts;
  ++s;
  *pts = r;
}
void eee::append(const IDraw* sh, p_t** ppts, size_t& s)
{
  extend(ppts, s, sh->begin());
  p_t b = sh->begin();
  while (sh->next(b) != sh->begin()) {
    b = sh->next(b);
    extend(ppts, s, b);
  }
}
void eee::paint(p_t p, char * cnv, f_t fr, char fill)
{
  size_t dx = p.x - fr.aa.x;
  size_t dy = fr.bb.y - p.y;
  cnv[dy * cols(fr) + dx] = fill;
}
void eee::flush(std::ostream& os, const char* cnv, f_t fr) {
  for (size_t i = 0; i < rows(fr); ++i) {
    for (size_t j = 0; j < cols(fr); ++j) {
      os << cnv[i * cols(fr) +j];
    }
    os << '\n';
  }
}
char * eee::canvas(f_t fr, char fill)
{
  size_t t = rows(fr) * cols(fr);
  char * c = new char[rows(fr) * cols(fr)];
  for (size_t i = 0; i < t; ++i) {
    c[i] = fill;
  }
  return c;
}
eee::f_t eee::frame(const p_t * pts, size_t s)
{
  int minx = pts[0].x, miny = pts[0].y;
  int maxx = minx, maxy = miny;
  for (size_t i = 0; i < s; ++i) {
    minx = std::min(minx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxx = std::max(maxx, pts[i].x);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t a{minx, miny};
  p_t b{maxx, maxy};
  return f_t{a, b};
}

eee::Triangle::Triangle(p_t hr): IDraw(), e{hr}
{}
eee::p_t eee::Triangle::begin() const {
  return e;
}
eee::p_t eee::Triangle::next(p_t prev) const {
  if (prev != e) {
    throw std::logic_error("bad prev");
  }
  return e;
}


eee::HLine::HLine(p_t hl): IDraw(), h{hl}
{}
eee::p_t eee::HLine::begin() const {
  return h;
}
eee::p_t eee::HLine::next(p_t prev) const {
  if (prev != h) {
    throw std::logic_error("bad prev");
  }
  return h;
}
eee::Dot::Dot(p_t dd): IDraw(), d{dd}
{}
eee::p_t eee::Dot::begin() const {
  return d;
}
eee::p_t eee::Dot::next(p_t prev) const {
  if (prev != d) {
    throw std::logic_error("bad prev");
  }
  return d;
}
size_t eee::rows(f_t fr) {
  return fr.bb.y - fr.aa.y + 1;
}
size_t eee::cols(f_t fr) {
  return fr.bb.x - fr.aa.x + 1;
}
bool eee::operator==(p_t a, p_t b) {
  return a.x == b.x && a.y == b.y;
}
bool eee::operator!=(p_t a, p_t b) {
  return !(a == b);
}


