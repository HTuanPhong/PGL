#include <pgl/pgl.h>

#include <SDL3/SDL_main.h>

static CStr vs = cstr_from_lit(
  "layout (location = 0) in vec2 aPos;\n"
  "void main()\n"
  "{\n"
  "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
  "}\n");

static CStr fs = cstr_from_lit(
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "    FragColor = vec4(1.0, 0.4, 0.2, 1.0);\n"
  "}\n");

static GLuint program, vao, vbo;

static void init() {
  ScratchMarker marker = scratch_begin(0);
  StrBuf        errLog = { 0 };
  program = shader_program_create(marker.scratch, vs, fs, &errLog);
  if (!program) {
    log_error("%s", errLog.value);
  }

  float vertices[] = {
    -0.6f, -0.4f,
    0.6f, -0.4f,
    0.0f, 0.6f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  scratch_end(marker);
}

static void frame(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);

  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  // Vec2I32 mouse_scroll = input_get_mouse_delta_scroll();
  // log_info("%d, %d %ld \n", mouse_scroll.x, mouse_scroll.y, __STDC_VERSION__);
}

static void tick() {
}

/* ===================== MAIN ===================== */

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  app_launch(cstr_from_lit("HELLO"), 800, 800, init, tick, frame);

  return 0;
}
