#ifndef DEPS_H
#define DEPS_H


// source https://schneide.wordpress.com/2016/07/15/generating-an-icosphere-in-c/
// source https://schneide.wordpress.com/2016/08/22/generating-a-spherified-cube-in-c/

struct v3
{
  v3(float x, float y, float z)
  {
    data[0]=x;
    data[1]=y;
    data[2]=z;
  }

  v3(float v=0.f) : v3(v,v,v)
  {
  }

  operator const float*() const
  {
    return data;
  }

  v3& operator+=(v3 const& rhs)
  {
    for (int i=0; i<3; ++i)
      data[i]+=rhs[i];
    return *this;
  }

  v3& operator-=(v3 const& rhs)
  {
    for (int i=0; i<3; ++i)
      data[i]-=rhs[i];
    return *this;
  }

  v3& operator*=(float rhs)
  {
    for (int i=0; i<3; ++i)
      data[i]*=rhs;
    return *this;
  }

  float squared() const
  {
    float result=0.f;
    for (int i=0; i<3; ++i)
      result+=data[i]*data[i];
    return result;
  }

  float data[3];
};

v3 operator+(v3 lhs, v3 const& rhs)
{
  return lhs+=rhs;
}

v3 operator-(v3 lhs, v3 const& rhs)
{
  return lhs-=rhs;
}

v3 operator*(v3 lhs, float rhs)
{
  return lhs*=rhs;
}

v3 operator/(v3 lhs, float rhs)
{
  return lhs*=(1.f/rhs);
}

v3 normalize(v3 rhs)
{
  return rhs/std::sqrt(rhs.squared());
}

using Index=std::uint32_t;

struct Triangle
{
  Index vertex[3];
};

struct ColorPosition
{
  v3 color;
  v3 position;
};

using TriangleList=std::vector<Triangle>;
using VertexList=std::vector<v3>;
using ColorVertexList=std::vector<ColorPosition>;

namespace color
{
v3 red{1.f, 0.f, 0.f};
v3 green{0.f, 1.f, 0.f};
v3 blue{0.f, 0.f, 1.f};
v3 yellow{1.f, 1.f, 0.f};
v3 cyan{0.f, 1.f, 1.f};
v3 purple{1.f, 0.f, 1.f};
};

namespace box_with_seams
{
const float D=1/std::sqrt(3.0f);
static const ColorVertexList vertices=
{
  {color::red,{-D,-D,-D}},{color::red,{D,-D,-D}},{color::red,{D,D,-D}},{color::red,{-D,D,-D}}, // back
  {color::green,{-D,-D,D}},{color::green,{D,-D,D}},{color::green,{D,D,D}},{color::green,{-D,D,D}}, // front
  {color::blue,{-D,-D,-D}},{color::blue,{-D,D,-D}},{color::blue,{-D,D,D}},{color::blue,{-D,-D,D}}, // left
  {color::yellow,{D,-D,-D}},{color::yellow,{D,D,-D}},{color::yellow,{D,D,D}},{color::yellow,{D,-D,D}}, // right
  {color::cyan,{-D,-D,-D}},{color::cyan,{D,-D,-D}},{color::cyan,{D,-D,D}},{color::cyan,{-D,-D,D}}, // bottom
  {color::purple,{-D,D,-D}},{color::purple,{D,D,-D}},{color::purple,{D,D,D}},{color::purple,{-D,D,D}}, // top
};

static const TriangleList triangles=
{
  {0, 1, 2}, {0, 2, 3},
  {4, 6, 5}, {4, 7, 6},
  {8, 9, 10}, {8, 10, 11},
  {12, 14, 13},{12, 15, 14},
  {16, 17, 18},{16, 18, 19},
  {20, 22, 21},{20, 23, 22}
};

}

using Lookup=std::map<std::pair<Index, Index>, Index>;

inline v3 split(v3 lhs, v3 rhs)
{
  return normalize(lhs+rhs);
}

inline ColorPosition split(ColorPosition lhs, ColorPosition rhs)
{
  return{(lhs.color+rhs.color)*0.5f, normalize(lhs.position+rhs.position)};
}

#endif // DEPS_H

