#pragma once

#include <gl_object.h>
#include <transformation.h>

struct duck {
  gl_object g;
  kaczka::mesh m;
  transformation t;
  GLuint ducktex;
  void load_from_file(const std::filesystem::path mesh,
                      const std::filesystem::path texture);
  duck();
  void render();
};
