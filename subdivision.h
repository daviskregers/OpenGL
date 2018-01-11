#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include <cmath>
#include <sdl2/include/SDL.h>
#include <gl/GLU.h>
#include <vector>
#include <tuple>
#include <map>
#include <array>
#include <deps.h>


template <typename VertexList>
Index vertex_for_edge(Lookup& lookup,
  VertexList& vertices, Index first, Index second)
{
  Lookup::key_type key(first, second);
  if (key.first>key.second)
    std::swap(key.first, key.second);

  auto inserted=lookup.insert({key, vertices.size()});
  if (inserted.second)
  {
    vertices.push_back(split(vertices[first], vertices[second]));
  }

  return inserted.first->second;
}

int longest_edge(ColorVertexList& vertices, Triangle const& triangle)
{
  float best=0.f;
  int result=0;
  for (int i=0; i<3; ++i)
  {
    auto a=vertices[triangle.vertex[i]].position;
    auto b=vertices[triangle.vertex[(i+1)%3]].position;
    float contest =(a-b).squared();
    if (contest>best)
    {
      best=contest;
      result=i;
    }
  }
  return result;
}

TriangleList subdivide_2(ColorVertexList& vertices,
                         TriangleList triangles)
{
  Lookup lookup;
  TriangleList result;

  for (auto&& each:triangles)
  {
    auto edge=longest_edge(vertices, each);
    Index mid=vertex_for_edge(lookup, vertices,
      each.vertex[edge], each.vertex[(edge+1)%3]);

    result.push_back({each.vertex[edge],
      mid, each.vertex[(edge+2)%3]});

    result.push_back({each.vertex[(edge+2)%3],
      mid, each.vertex[(edge+1)%3]});
  }

  return result;
}

using IndexedMesh=std::pair<VertexList, TriangleList>;
using ColoredIndexMesh=std::pair<ColorVertexList, TriangleList>;

ColoredIndexMesh make_spherified_cube_seams(int subdivisions)
{
  ColorVertexList vertices=box_with_seams::vertices;
  TriangleList triangles=box_with_seams::triangles;

  for (int i=0; i<subdivisions; ++i)
  {
    triangles=subdivide_2(vertices, triangles);
  }

  return{vertices, triangles};
}

void RenderMesh(ColorVertexList const& vertices, TriangleList const& triangles)
{

    glDisable(GL_CULL_FACE);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glBegin(GL_TRIANGLES);

    for (auto&& triangle:triangles)
    {
      for (int c=0; c<3; ++c)
      {
        auto const& vertex=vertices[triangle.vertex[c]];
        glColor3fv(vertex.color);
        glVertex3fv(vertex.position);
      }
    }

    glEnd();

    glEnable(GL_CULL_FACE);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}

template <class VertexList>
void Render(float angle, VertexList const& vertices, TriangleList const& triangles)
{
  RenderMesh(vertices, triangles);
}

#endif // SUBDIVISION_H
