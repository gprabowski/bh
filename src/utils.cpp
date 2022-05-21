#include <cstdlib>
#include <istream>
#include <set>

#include <glad/glad.h>

#define GLM_FORCE_RADIANS
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <frame_state.h>
#include <gl_object.h>
#include <log.h>
#include <utils.h>

#include <mtxlib.h>

namespace utils {

void get_model_uniform(transformation &t, glm::mat4 &out) {
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);

  const auto trans = glm::translate(glm::mat4(1.0f), t.translation);
  const auto scale = glm::scale(glm::mat4(1.0f), t.scale);
  const auto rot = glm::toMat4(glm::quat(glm::radians(t.rotation)));

  const auto model = trans * scale * rot;

  out = model;
}

void set_model_uniform(transformation &t) {
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);

  const auto trans = glm::translate(glm::mat4(1.0f), t.translation);
  const auto scale = glm::scale(glm::mat4(1.0f), t.scale);
  const auto rot = glm::toMat4(glm::quat(glm::radians(t.rotation)));

  const auto model = trans * scale * rot;

  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
}

void set_lighting_uniforms(GLfloat ambient, GLfloat diffuse, GLfloat specular) {
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  glUniform3f(glGetUniformLocation(program, "intensity"), ambient, diffuse,
              specular);
}

} // namespace utils
