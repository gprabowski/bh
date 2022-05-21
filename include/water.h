#pragma once

#include <glad/glad.h>

#include <gl_object.h>
#include <mesh.h>
#include <shader_manager.h>
#include <transformation.h>

struct water {
  GLuint height_textures[2];
  GLuint normal_texture;
  GLuint water_depth;
  GLuint water_framebuffer;
  shader regeneration_program;

  gl_object g;
  kaczka::mesh m;
  transformation t;

  water();
  ~water();
  void drop_droplet();
  void regenerate();
  void render();
};
