#include <water.h>

#include <frame_state.h>
#include <framebuffer.h>
#include <log.h>
#include <shader_manager.h>
#include <utils.h>

void water::drop_droplet() {}
void water::regenerate() {
  glViewport(0, 0, 256, 256);
  glNamedFramebufferTexture(water_framebuffer, GL_COLOR_ATTACHMENT0,
                            height_textures[1], 0);
  glBindFramebuffer(GL_FRAMEBUFFER, water_framebuffer);
  // bind height texture 0 as source

  glBindVertexArray(g.vao);
  glUseProgram(regeneration_program.idx);
  glBindTextureUnit(2, height_textures[0]);
  glUniform1i(glGetUniformLocation(regeneration_program.idx, "height_texture"),
              2);

  // bind height texture 1 as output 0
  // set program to generation program
  // render
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, m.elements.size(), GL_UNSIGNED_INT, NULL);
  //   restore framebuffer state
  std::swap(height_textures[0], height_textures[1]);
  glViewport(0, 0, frame_state::window_w, frame_state::window_h);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void water::render() {
  glBindVertexArray(g.vao);
  glUseProgram(g.program);

  glBindTextureUnit(2, height_textures[0]);
  glBindTextureUnit(3, normal_texture);

  glUniform1i(glGetUniformLocation(g.program, "height_map"), 2);
  glUniform1i(glGetUniformLocation(g.program, "normal_map"), 3);

  utils::set_model_uniform(t);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, m.elements.size(), GL_UNSIGNED_INT, NULL);
}

water::water() {
  static auto &sm = shader_manager::get_manager();
  // init height_textures[2];
  glCreateTextures(GL_TEXTURE_2D, 2, height_textures);

  // color
  glTextureParameteri(height_textures[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(height_textures[0], GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(height_textures[0], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(height_textures[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureStorage2D(height_textures[0], 1, GL_RGBA8, 256, 256);

  glTextureParameteri(height_textures[1], GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(height_textures[1], GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(height_textures[1], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(height_textures[1], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureStorage2D(height_textures[1], 1, GL_RGBA8, 256, 256);

  // init normal_texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &normal_texture);
  glTextureParameteri(normal_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(normal_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(normal_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(normal_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureStorage2D(normal_texture, 1, GL_RGBA8, 256, 256);

  // init gl_object g
  // GEOMETRY
  // generate vertices
  m.vertices = {
      // south wall
      {{-1.0f, -1.0f, 0.f}, {0.0f, 0.0f, 0.0f}},
      {{-1.0f, 1.0f, 0.f}, {0.0f, 1.0f, 0.0f}},
      {{1.0f, 1.0f, 0.f}, {1.0f, 1.0f, 0.0f}},
      {{1.0f, -1.0f, 0.f}, {1.0f, 0.0f, 0.0f}},
  };

  // generate indices
  m.elements = {0, 2, 1, 2, 0, 3};

  g.program = sm.programs[shader_t::WATER_SHADER].idx;
  g.reset_api_elements(m);
  // init_transform t
  glCreateFramebuffers(1, &water_framebuffer);

  glCreateTextures(GL_TEXTURE_2D, 1, &water_depth);

  // depth
  glTextureParameteri(water_depth, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(water_depth, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(water_depth, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(water_depth, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureStorage2D(water_depth, 1, GL_DEPTH24_STENCIL8, 256, 256);

  regeneration_program = sm.programs[shader_t::WATER_GENERATION_SHADER];
  t.rotation.x = -90;
  t.translation.y = -0.2;
}

water::~water() {}
