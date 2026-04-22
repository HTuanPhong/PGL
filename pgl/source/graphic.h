#ifndef PGL_INTERNAL_GRAPHIC_H
#define PGL_INTERNAL_GRAPHIC_H

#include <pgl/graphic.h>
#include <pgl/math.h>

typedef struct UIGraphic {
  struct Rect {
    GLuint       vao;
    GLuint       vbo;
    GLuint       shader_program;
    DynamicBlock buffer;
  } rect;
  struct Blur {
    GLuint       vao;
    GLuint       vbo;
    GLuint       downsample_program;
    GLuint       upsample_program;
    GLuint       fbo[2];
    GLuint       fbo_tex[2];
    DynamicBlock buffer;
  } blur;
} UIGraphic;

typedef struct BlurInstance {
  Vec4F32 rect;
  Vec4F32 radii;
} BlurInstance;

typedef struct RectInstance {
  Vec4F32 rect;      // topright, topright, bottomleft, bottomleft
  Vec4F32 uvs;       // u, v, u2, v2
  Vec4F32 colors[4]; // 4 corners
  Vec4F32 radii;     // 4 corners
  struct Style {
    f32 border;
    f32 softness;
    f32 omit_tex;
    f32 shear;
  } style;
} RectInstance;

void create_ui_graphic();
void draw_ui_graphic();

#endif // PGL_INTERNAL_GRAPHIC_H
