#include <duck.h>

#include <filesystem>
#include <fstream>
#include <vector>

#include <stb/stb_image.h>

#include <log.h>
#include <shader_manager.h>
#include <utils.h>

std::vector<std::string> read_mesh_file(const std::filesystem::path mesh_file) {
  std::vector<std::string> ret;
  std::ifstream ifs;

  auto ex = ifs.exceptions();
  ex |= std::ios_base::badbit | std::ios_base::failbit;
  // ifs.exceptions(ex);

  ifs.open(mesh_file);
  ifs.ignore(std::numeric_limits<std::streamsize>::max());
  auto size = ifs.gcount();
  GK2_KACZKA_INFO("[MESH] Read {0} bytes from {1}", size, mesh_file.c_str());

  ifs.clear();
  ifs.seekg(0, std::ios_base::beg);

  std::string current;
  while (getline(ifs, current)) {
    ret.push_back(current);
  }

  return ret;
}

void duck::load_from_file(const std::filesystem::path mesh,
                          const std::filesystem::path texture) {

  auto &sm = shader_manager::get_manager();
  std::stringstream ss;
  auto lines = read_mesh_file(mesh);

  std::size_t curr_idx{0u};

  // read all vertices
  std::size_t num_vertices{0u};
  ss << lines[curr_idx++];
  ss >> num_vertices;
  ss.clear();
  for (std::size_t vi = 0u; vi < num_vertices; ++vi) {
    float x, y, z, nx, ny, nz, tu, tv;
    ss << lines[curr_idx++];
    ss >> x >> y >> z >> nx >> ny >> nz >> tu >> tv;
    // add to mesh
    m.vertices.emplace_back(kaczka::vertex_t{
        glm::vec3{x, y, z}, glm::vec3{nx, ny, nz}, glm::vec2{tu, tv}});
    ss.clear();
  }

  // read all triangles
  std::size_t num_triangles{0u};
  ss << lines[curr_idx++];
  ss >> num_triangles;
  ss.clear();
  for (std::size_t vi = 0u; vi < num_triangles; ++vi) {
    GLuint one, two, three;
    ss << lines[curr_idx++];
    ss >> one >> two >> three;
    // add to mesh
    m.elements.emplace_back(one);
    m.elements.emplace_back(two);
    m.elements.emplace_back(three);
    ss.clear();
  }

  g.program = sm.programs[shader_t::DUCK_SHADER].idx;
  g.reset_api_elements(m);

  unsigned char *data;
  int width, height, nr_channels;

  glCreateTextures(GL_TEXTURE_2D, 1, &ducktex);

  glTextureParameteri(ducktex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(ducktex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(ducktex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(ducktex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load(texture.c_str(), &width, &height, &nr_channels, 0);

  glTextureStorage2D(ducktex, 1, GL_RGBA8, width, height);

  glTextureSubImage2D(ducktex, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE,
                      data);

  stbi_image_free(data);

  t.scale = {0.002, 0.002, 0.002};
}

duck::duck() { load_from_file("assets/duck.txt", "assets/ducktex.jpg"); }

void duck::render() {

  glBindVertexArray(g.vao);
  glUseProgram(g.program);

  glBindTextureUnit(2, ducktex);

  glUniform1i(glGetUniformLocation(g.program, "duck_map"), 2);

  utils::set_model_uniform(t);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, m.elements.size(), GL_UNSIGNED_INT, NULL);
}
