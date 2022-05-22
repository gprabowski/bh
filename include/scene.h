#pragma once

#include <array>

#define GLM_FORCE_RADIANS
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cube.h>
#include <duck.h>
#include <gl_object.h>
#include <transformation.h>
#include <water.h>

namespace kaczka {

struct scene {
  duck d;
  water w;
  cube c;

  void before();
  void draw();
  void after();

  scene();

  // various render passes
private:
};

}; // namespace kaczka
