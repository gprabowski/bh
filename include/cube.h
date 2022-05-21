#pragma once

#include <gl_object.h>
#include <mesh.h>
#include <transformation.h>

struct cube {
  GLuint cube_map;
  gl_object g;
  transformation t;
  kaczka::mesh m;
  bool visible{true};

  cube();
  ~cube();
  void generate();
  void render();
};
