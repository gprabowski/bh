#include <log.h>
#include <scene.h>
#include <utils.h>

#include <framebuffer.h>
#include <fstream>
#include <iostream>
#include <shader_manager.h>
#include <sstream>
#include <string>
#include <vector>

#include <frame_state.h>
#include <frame_update.h>

void decompose(const glm::mat4 &m, glm::vec3 &trans, glm::vec3 &scale,
               glm::vec3 &rot) {
  trans = glm::vec3(m[3]);
  scale = {glm::length(glm::vec3(m[0])), glm::length(glm::vec3(m[1])),
           glm::length(glm::vec3(m[2]))};

  glm::mat4 m_rot(m[0] / scale.x, m[1] / scale.y, m[2] / scale.z,
                  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  rot = glm::degrees(glm::eulerAngles(glm::quat_cast(m_rot)));
}

void kaczka::scene::draw() {
  update::setup_globals();
  update::refresh_ubos();

  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  w.render();
  c.render();
}

void load_from_file(const std::filesystem::path mesh,
                    const std::filesystem::path texture) {}
