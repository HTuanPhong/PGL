#ifndef PGL_INTERNAL_GRAPHIC_H
#define PGL_INTERNAL_GRAPHIC_H

#include <pgl/graphic.h>
#include <pgl/math.h>

typedef struct UIGraphic {
  struct Rect {
    GLuint       vao;
    GLuint       vbo;
    GLuint       shaderProgram;
    DynamicBlock buffer;
  } rect;
  struct Blur {
    GLuint       vao;
    GLuint       vbo;
    GLuint       DownsampleProgram;
    GLuint       UpsampleProgram;
    GLuint       fbo[2];
    GLuint       fboTex[2];
    DynamicBlock buffer;
  } blur;
} UIGraphic;

typedef struct BlurInstance {
  Vec4F rect;
  Vec4F radii;
} BlurInstance;

typedef struct RectInstance {
  Vec4F rect;      // topright, topright, bottomleft, bottomleft
  Vec4F uvs;       // u, v, u2, v2
  Vec4F colors[4]; // 4 corners
  Vec4F radii;     // 4 corners
  struct Style {
    float border;
    float softness;
    float omitTex;
    float shear;
  } style;
} RectInstance;

void CreateUIGraphic();
void DrawUIGraphic();

#endif // PGL_INTERNAL_GRAPHIC_H
