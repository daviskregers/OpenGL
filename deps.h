#ifndef DEPS_H
#define DEPS_H

#include <glm/glm/glm.hpp>

// source https://schneide.wordpress.com/2016/07/15/generating-an-icosphere-in-c/
// source https://schneide.wordpress.com/2016/08/22/generating-a-spherified-cube-in-c/

struct Triangle
{
  unsigned int vertex[3];
};

using TriangleList=std::vector<Triangle>;
using VertexList=std::vector<glm::vec3>;
using ColorVertexList=std::vector<glm::vec3>;

namespace box_with_seams
{
    const float D=1/std::sqrt(3.0f);
    static const ColorVertexList vertices=
    {
        {-D,-D,-D},{D,-D,-D},{D,D,-D},{-D,D,-D}, // back
        {-D,-D,D},{D,-D,D},{D,D,D},{-D,D,D}, // front
        {-D,-D,-D},{-D,D,-D},{-D,D,D},{-D,-D,D}, // left
        {D,-D,-D},{D,D,-D},{D,D,D},{D,-D,D}, // right
        {-D,-D,-D},{D,-D,-D},{D,-D,D},{-D,-D,D}, // bottom
        {-D,D,-D},{D,D,-D},{D,D,D},{-D,D,D}, // top
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

    using Lookup=std::map<std::pair<unsigned int, unsigned int>, unsigned int>;

    inline glm::vec3 split(glm::vec3 lhs, glm::vec3 rhs)
    {
      return glm::normalize(lhs+rhs);
    }

#endif // DEPS_H

