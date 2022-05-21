#pragma once

#include <glad/glad.h>

#include <gl_object.h>
#include <transformation.h>

struct water {
  GLuint height_textures[2];
  GLuint normal_texture;

  gl_object g;
  transformation t;

  void drop_droplet();
  void regenerate();
  void render();
};
