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

void blackhole::scene::draw() {
  update::setup_globals();
  update::refresh_ubos();

  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  c.render();
}

void load_from_file(const std::filesystem::path mesh,
                    const std::filesystem::path texture) {}

blackhole::scene::scene() {}
