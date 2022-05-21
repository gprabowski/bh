#pragma once

#include <gl_object.h>
#include <input_state.h>
#include <memory>
#include <mesh.h>
#include <transformation.h>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <mtxlib.h>

namespace utils {

void set_model_uniform(transformation &t);
void set_lighting_uniforms(GLfloat ambient, GLfloat diffuse, GLfloat specular);
void get_model_uniform(transformation &t, glm::mat4 &out);
void refresh_common_uniforms(GLuint program);

void inverse_kinematics(vector3 pos, vector3 normal, float &a1, float &a2,
                        float &a3, float &a4, float &a5);

template <typename O> void render_primitives(O &o, GLenum primitives) {
  if (!o.visible) {
    return;
  }
}

} // namespace utils
