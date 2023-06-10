#include <cube.h>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <shader_manager.h>
#include <utils.h>

#include <iostream>

struct image_data {
  unsigned char *data;
  int width, height, nr_channels;
};

cube::cube() {
  // initialize cube map texture
  std::array<std::string, 6> textures_faces{
      "assets/skybox/right.png", "assets/skybox/left.png",
      "assets/skybox/top.png",   "assets/skybox/bottom.png",
      "assets/skybox/front.png", "assets/skybox/back.png",
  };

  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cube_map);
  std::array<image_data, 6> data;

  for (std::size_t i = 0; i < data.size(); ++i) {
    data[i].data = stbi_load(textures_faces[i].c_str(), &data[i].width,
                             &data[i].height, &data[i].nr_channels, 3);
  }

  glTextureStorage2D(cube_map, 1, GL_RGB8, data[0].width, data[0].height);

  for (std::size_t i = 0; i < textures_faces.size(); ++i) {
    std::cout << " size: " << data[i].width << " " << data[i].height
              << std::endl;
    glTextureSubImage3D(cube_map, 0, 0, 0, i, data[i].width, data[i].height, 1,
                        GL_RGB, GL_UNSIGNED_BYTE, data[i].data);
  }

  for (std::size_t i = 0; i < data.size(); ++i) {
    stbi_image_free(data[i].data);
  }

  glTextureParameteri(cube_map, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(cube_map, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(cube_map, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTextureParameteri(cube_map, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(cube_map, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // GEOMETRY
  static auto &sm = shader_manager::get_manager();
  // generate vertices
  m.vertices = {

      // south wall
      {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

  };

  // generate indices
  m.elements = {0, 2, 1, 3, 2, 0};

  g.program = sm.programs[shader_t::CUBE_SHADER].idx;
  g.reset_api_elements(m);
}

cube::~cube() {}

void cube::generate() {}

void cube::render() {
  glBindVertexArray(g.vao);
  glUseProgram(g.program);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map);
  utils::set_model_uniform(t);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, m.elements.size(), GL_UNSIGNED_INT, NULL);
}
