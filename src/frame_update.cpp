#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <frame_state.h>
#include <frame_update.h>
#include <input_state.h>
#include <shader_manager.h>
#include <utils.h>

#include <mtxlib.h>

namespace update {
void setup_globals() {
  auto &state = input_state::get_input_state();

  int width, height;
  auto w = glfwGetCurrentContext();
  glfwGetFramebufferSize(w, &width, &height);

  frame_state::window_h = height;
  frame_state::window_w = width;
  const auto min = ImGui::GetWindowContentRegionMin();
  const auto max = ImGui::GetWindowContentRegionMax();
  frame_state::content_area = {max.x - min.x, max.y - min.y};
  frame_state::content_pos = {ImGui::GetWindowPos()};
  frame_state::content_pos = {frame_state::content_pos.x + min.x,
                              frame_state::content_pos.y + min.y};

  auto view = (glm::lookAt(state.cam_pos, state.cam_pos + state.cam_front,
                           state.cam_up));
  auto proj =
      (glm::perspective(45.f,
                        static_cast<float>(frame_state::content_area.x) /
                            frame_state::content_area.y,
                        0.1f, 1000.f));

  frame_state::view = view;
  frame_state::proj = proj;
}

void refresh_ubos() {
  static auto &sm = shader_manager::get_manager();
  static auto &is = input_state::get_input_state();
  glBindBufferBase(GL_UNIFORM_BUFFER, sm.common_ubo_block_loc, sm.common_ubo);
  float *common_ubo_ptr = (float *)glMapNamedBufferRange(
      sm.common_ubo, 0, sizeof(float) * (2 * 16 + 3 * 4),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  memcpy(&common_ubo_ptr[0], glm::value_ptr(frame_state::proj),
         sizeof(float) * 16);
  memcpy(&common_ubo_ptr[16], glm::value_ptr(frame_state::view),
         sizeof(float) * 16);
  memcpy(&common_ubo_ptr[32], glm::value_ptr(frame_state::light_pos),
         sizeof(float) * 4);
  memcpy(&common_ubo_ptr[36], glm::value_ptr(frame_state::light_color),
         sizeof(float) * 4);
  memcpy(&common_ubo_ptr[40], glm::value_ptr(is.cam_pos), sizeof(float) * 4);
  glUnmapNamedBuffer(sm.common_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, sm.common_ubo);
}

void refresh_view(const glm::mat4 &view) {
  static auto &sm = shader_manager::get_manager();
  static auto &is = input_state::get_input_state();
  glBindBufferBase(GL_UNIFORM_BUFFER, sm.common_ubo_block_loc, sm.common_ubo);
  float *common_ubo_ptr = (float *)glMapNamedBufferRange(
      sm.common_ubo, 0, sizeof(float) * (2 * 16 + 3 * 4),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  memcpy(&common_ubo_ptr[0], glm::value_ptr(frame_state::proj),
         sizeof(float) * 16);
  memcpy(&common_ubo_ptr[16], glm::value_ptr(view), sizeof(float) * 16);
  memcpy(&common_ubo_ptr[32], glm::value_ptr(frame_state::light_pos),
         sizeof(float) * 4);
  memcpy(&common_ubo_ptr[36], glm::value_ptr(frame_state::light_color),
         sizeof(float) * 4);
  memcpy(&common_ubo_ptr[40], glm::value_ptr(is.cam_pos), sizeof(float) * 4);
  glUnmapNamedBuffer(sm.common_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, sm.common_ubo);
}

void per_frame_update(kaczka::scene &s) {
  static auto last_tick = glfwGetTime();
  auto current_tick = glfwGetTime();
  auto delta = last_tick - current_tick;
  s.d.update();
  s.w.regenerate({s.d.t.translation.x, -s.d.t.translation.z});
  last_tick = current_tick;
  delta = delta + 0.f;
}

} // namespace update
