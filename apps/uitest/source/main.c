#include "pgl/log.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <pgl/pgl.h>

#define GLSL(src) #src

static const char *vs =
  "layout (location = 0) in vec2 aPos;\n"
  "void main()\n"
  "{\n"
  "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
  "}\n";

static const char *fs =
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "    FragColor = vec4(1.0, 0.4, 0.2, 1.0);\n"
  "}\n";

static GLuint program, vao, vbo;

static void init() {
  ScratchMarker marker = ScratchBegin(0);
  String        errLog = { 0 };
  program = CreateShaderProgram(marker.scratch, vs, fs, &errLog);
  if (!program) {
    LogErr("%s", errLog.value);
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

  ScratchEnd(marker);
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
  LogInfo("%d, %d \n", app.input.mouse.deltaScrollPixel.x, app.input.mouse.deltaScrollPixel.y);
  SDL_Delay(1000);
}

static void tick() {
}

/* ===================== MAIN ===================== */

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  LaunchApp("HELLO", 800, 800, init, tick, frame);

  return 0;
}
