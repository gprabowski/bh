#pragma once

#include <random>

#include <gl_object.h>
#include <transformation.h>

struct movement_spline {
  std::array<glm::vec3, 4> control_points;
  std::array<glm::vec3, 3> derivative_points;
};

struct duck {
  std::random_device r{};
  std::mt19937 mt{r()};
  std::uniform_real_distribution<float> dist{0.0, 1.0};
  float current_t = 0.0f;
  bool animation{true};
  glm::vec4 strain_dir{sqrtf(2.f), 0, -sqrtf(2.f), 1.0f};

  gl_object g;
  kaczka::mesh m;
  transformation t;
  GLuint ducktex;
  movement_spline bspline;

  void load_from_file(const std::filesystem::path mesh,
                      const std::filesystem::path texture);
  void update();
  duck();
  void render();
  glm::vec3 get_point() {
    return glm::vec3{dist(mt) * 2 - 1.0, -0.2, dist(mt) * 2 - 1.0};
  };
};
