#pragma once

#include <gl_object.h>
#include <transformation.h>

struct duck {
  gl_object g;
  transformation t;
  void load_from_file(const std::filesystem::path mesh,
                      const std::filesystem::path texture);
};
