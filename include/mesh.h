#pragma once
#include <array>
#include <filesystem>
#include <vector>

#include <cassert>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

namespace kaczka {

struct vertex_t {
  glm::vec3 pos;
  glm::vec3 norm;
  glm::vec2 tex;
};

struct mesh {
  std::vector<vertex_t> vertices;
  std::vector<GLuint> elements;
};

} // namespace kaczka
