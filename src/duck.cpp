#include <duck.h>

#include <filesystem>
#include <fstream>
#include <random>
#include <vector>

#include <stb/stb_image.h>

#include <log.h>
#include <shader_manager.h>
#include <utils.h>

void decompose(const glm::mat4 &m, glm::vec3 &trans, glm::vec3 &scale,
               glm::vec3 &rot) {
  trans = glm::vec3(m[3]);
  scale = {glm::length(glm::vec3(m[0])), glm::length(glm::vec3(m[1])),
           glm::length(glm::vec3(m[2]))};

  glm::mat4 m_rot(m[0] / scale.x, m[1] / scale.y, m[2] / scale.z,
                  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  rot = glm::degrees(glm::eulerAngles(glm::quat_cast(m_rot)));
}

glm::vec3 deboor(const std::array<glm::vec3, 4> &ms, float t) {
  const glm::vec3 &P10 = ms[0];
  const glm::vec3 &P20 = ms[1];
  const glm::vec3 &P30 = ms[2];
  const glm::vec3 &P40 = ms[3];

  const float u = 2.f + t;

  const float a41 = (u - 2.0f) / 3.0f;
  const float a31 = (u - 1.0f) / 3.0f;
  const float a21 = (u) / 3.0f;

  const float b41 = 1.0f - a41;
  const float b31 = 1.0f - a31;
  const float b21 = 1.0f - a21;

  const glm::vec3 P41 = a41 * P40 + b41 * P30;
  const glm::vec3 P31 = a31 * P30 + b31 * P20;
  const glm::vec3 P21 = a21 * P20 + b21 * P10;

  const float a42 = (u - 2.0f) / 2.0f;
  const float a32 = (u - 1.0f) / 2.0f;

  const float b42 = 1.0f - a42;
  const float b32 = 1.0f - a32;

  const glm::vec3 P42 = a42 * P41 + b42 * P31;
  const glm::vec3 P32 = a32 * P31 + b32 * P21;

  const float a43 = (u - 2.0f) / 1.0f;
  const float b43 = 1.0f - a43;

  const glm::vec3 P43 = a43 * P42 + b43 * P32;

  return P43;
}

glm::vec3 deboor_derivative(const std::array<glm::vec3, 3> &ms, float t) {
  const glm::vec3 &P10 = ms[0];
  const glm::vec3 &P20 = ms[1];
  const glm::vec3 &P30 = ms[2];

  const float u = 1.f + t;

  const float a31 = (u - 1.0f) / 2.0f;
  const float a21 = (u) / 2.0f;

  const float b31 = 1.0f - a31;
  const float b21 = 1.0f - a21;

  const glm::vec3 P31 = a31 * P30 + b31 * P20;
  const glm::vec3 P21 = a21 * P20 + b21 * P10;

  const float a32 = (u - 1.0f) / 1.0f;
  const float b32 = 1.0f - a32;

  const glm::vec3 P32 = a32 * P31 + b32 * P21;

  return P32;
}

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
  t.translation = {0.0, -0.2, 0.0};

  //
}

duck::duck() {
  load_from_file("assets/duck.txt", "assets/ducktex.jpg");
  // initialize spline with points from -1 to 1 everywhere

  bspline.control_points = {get_point(), get_point(), get_point(), get_point()};

  auto d1 = bspline.control_points[1] - bspline.control_points[0];
  auto d2 = bspline.control_points[2] - bspline.control_points[1];
  auto d3 = bspline.control_points[3] - bspline.control_points[2];

  bspline.derivative_points = {d1, d2, d3};
}

void duck::render() {

  glBindVertexArray(g.vao);
  glUseProgram(g.program);

  glBindTextureUnit(2, ducktex);

  glUniform1i(glGetUniformLocation(g.program, "duck_map"), 2);

  auto translation = glm::vec4(deboor(bspline.control_points, current_t), 1.0);
  auto tangent =
      -1.f *
      glm::normalize(deboor_derivative(bspline.derivative_points, current_t));
  auto up = glm::vec3(0.0, 1.0, 0.0);
  auto bitangent = glm::normalize(glm::cross(tangent, up));
  glm::mat4 total_transformation(glm::vec4(tangent, 0.0), glm::vec4(up, 0.0),
                                 glm::vec4(bitangent, 0.0), translation);
  glm::mat4 final =
      total_transformation * glm::scale(glm::mat4(1.0f), {0.002, 0.002, 0.002});

  utils::set_model_uniform(final);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, m.elements.size(), GL_UNSIGNED_INT, NULL);
}

void duck::update() {
  static double last_time = glfwGetTime();

  float dt = static_cast<float>(glfwGetTime() - last_time);
  last_time = glfwGetTime();

  float new_t = current_t + dt;

  if (new_t > 1.0) {
    std::swap(bspline.control_points[0], bspline.control_points[1]);
    std::swap(bspline.control_points[1], bspline.control_points[2]);
    std::swap(bspline.control_points[2], bspline.control_points[3]);
    bspline.control_points[3] = get_point();

    // regenerate derivatives
    auto d1 = bspline.control_points[1] - bspline.control_points[0];
    auto d2 = bspline.control_points[2] - bspline.control_points[1];
    auto d3 = bspline.control_points[3] - bspline.control_points[2];

    bspline.derivative_points = {d1, d2, d3};

    current_t = new_t - 1.0f;
  } else {
    current_t = new_t;
  }
  t.translation = deboor(bspline.control_points, current_t);
}
