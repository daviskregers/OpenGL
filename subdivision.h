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

#include <mesh.h>


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
using ColoredIndexMesh=std::pair<std::vector<Vertex>, std::vector<unsigned int>>;

ColoredIndexMesh make_spherified_cube_seams(int subdivisions)
{
  ColorVertexList vertices=box_with_seams::vertices;
  TriangleList triangles=box_with_seams::triangles;

  std::vector<Vertex> verts;
  std::vector<unsigned int> indices;

  for (int i=0; i<subdivisions; ++i)
  {
    triangles=subdivide_2(vertices, triangles);
  }

  float u = 1/vertices.size(), v = 1/vertices.size();

  for(auto i = vertices.begin(); i < vertices.end(); ++i) {

      glm::vec3 n;

      verts.push_back(Vertex( glm::vec3(i->position.data[0], i->position.data[1], i->position.data[2]), glm::vec2(u,v), n ));

  }

  for( auto i = triangles.begin(); i < triangles.end(); ++i) {

      glm::vec3 a = glm::vec3(vertices[i->vertex[0]].position.data[0], vertices[i->vertex[0]].position.data[1], vertices[i->vertex[0]].position.data[2]);
      glm::vec3 b = glm::vec3(vertices[i->vertex[1]].position.data[0], vertices[i->vertex[1]].position.data[1], vertices[i->vertex[1]].position.data[2]);
      glm::vec3 c = glm::vec3(vertices[i->vertex[2]].position.data[0], vertices[i->vertex[2]].position.data[1], vertices[i->vertex[2]].position.data[2]);
      glm::vec3 n = glm::normalize(glm::cross(c - a, b - a));

      verts[i->vertex[0]].norm = n;
      verts[i->vertex[1]].norm = n;
      verts[i->vertex[2]].norm = n;

      indices.push_back(i->vertex[0]);
      indices.push_back(i->vertex[1]);
      indices.push_back(i->vertex[2]);

  }

  return{verts, indices};
}

#endif // SUBDIVISION_H
