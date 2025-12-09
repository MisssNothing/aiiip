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
    p_t begin() const override;
    p_t next(p_t prev) const override;
    f_t rect;
  };
  struct Frect: IDraw {
    Frect(p_t pos, int w, int h);
    Frect(p_t a, p_t b);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    f_t rect;
  };
  struct Layers {
    Layers();
    ~Layers();
    Layers(const Layers&) = delete;
    Layers operator=(const Layers&) = delete;
    Layers(Layers&&) = delete;
    Layers& operator=(Layers&&) = delete;
    void append(const IDraw & dr);
    size_t layers() const;
    size_t points() const;
    size_t start(size_t i) const;
    size_t end(size_t i) const;
    p_t point(size_t i) const;
    f_t frame() const;
  private:
    size_t points_;
    p_t * pts_;
    size_t layers_;
    size_t * sizes_;
  };
  p_t * extend(const p_t* pts, size_t s, p_t fill);
  void extend(p_t** pts, size_t& s, p_t fill);
  void append(const IDraw* sh, p_t** ppts, size_t& s);
  f_t frame(const p_t * pts, size_t s);
  struct Layers;
  f_t frame(const Layers& ls);
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
  IDraw* shp[3] = {};
  Layers layers;
  try {
    shp[0] = new Rect({10, 10}, {20, 20});
    shp[1] = new Frect({0, 0}, {3, 4});
    shp[2] = new Dot({1, 9});
    for (size_t i = 0; i < 3; ++ i) {
      layers.append(*(shp[i]));
    }
    f_t fr = frame(layers);
    char * cnv = canvas(fr, '.');
    const char * brush = "%0#";
    for (size_t k = 0; k < layers.layers(); ++k) {
      size_t start = layers.start(k);
      size_t end = layers.end(k);
      for (size_t i = start; i < end; ++i) {
        paint(layers.point(i), cnv, fr, brush[k]);
      }
    }
    flush(std::cout, cnv, fr);
    delete [] cnv;
  } catch (...) {
    std::cerr << "Error\n";
    err = 1;
  }
  for (int i = 0; i < 3; ++i) {
    delete shp[i];
  }
  return err;
}
eee::Layers::Layers():
  points_{0},
  pts_{nullptr},
  layers_{0},
  sizes_{nullptr}
{}
void eee::Layers::append(const IDraw& dr) {
  size_t* ext_sizes = new size_t[layers_ + 1];
  try {
    eee::append(&dr, &pts_, points_);
  } catch (...) {
    delete [] ext_sizes;
    throw;
  }
  for (size_t i = 0; i < layers_; ++i) {
    ext_sizes[i] = sizes_[i];
  }
  ext_sizes[layers_] = points_;
  delete [] sizes_;
  sizes_ = ext_sizes;
  ++layers_;
}
eee::Layers::~Layers()
{
  delete [] pts_;
  delete [] sizes_;
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

eee::Frect::Frect(p_t pos, int w, int h):
  IDraw(),
  rect{pos, {pos.x + w, pos.y + h}}
{
  if (!(w > 0 && h > 0)) {
    throw std::logic_error("bad rect");
  }
}
eee::Frect::Frect(p_t a, p_t b):
  Frect(a, b.x - a.x, b.y - a.y)
{}
eee::p_t eee::Frect::begin() const {
  return rect.aa;
}
eee::p_t eee::Frect::next(p_t prev) const {
  if (prev.x < rect.bb.x) {
    return {prev.x + 1, prev.y};
  } else if (prev.x == rect.bb.x && prev.y < rect.bb.y) {
    return {rect.aa.x, prev.y + 1};
  } else if (prev == rect.bb) {
    return rect.aa;
  }
  throw std::logic_error("bad impl");
}

eee::Rect::Rect(p_t pos, int w, int h):
  IDraw(),
  rect{pos, {pos.x + w, pos.y + h}}
{
  if (!(w > 0 && h > 0)) {
    throw std::logic_error("bad rect");
  }
}
eee::Rect::Rect(p_t a, p_t b): Rect(a, b.x - a.x, b.y - a.y)
{}
eee::p_t eee::Rect::begin() const {
  return rect.aa;
}
eee::p_t eee::Rect::next(p_t prev) const {
  if (prev.x == rect.aa.x && prev.y < rect.bb.y) {
    return {prev.x, prev.y + 1};
  } else if (prev.y == rect.bb.y && prev.x < rect.bb.x) {
    return {prev.x + 1, prev.y};
  } else if (prev.x == rect.bb.x && prev.y > rect.aa.y) {
    return {prev.x, prev.y - 1};
  } else if (prev.y == rect.aa.y && prev.x > rect.aa.x) {
    return {prev.x - 1, prev.y};
  }
  throw std::logic_error("bad impl");
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


//сделать перемещение и копирование и реал методы struct Layers
