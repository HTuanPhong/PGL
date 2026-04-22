#include "graphic.h"
#include <pgl/app.h>
#include <pgl/graphic.h>
#include <pgl/log.h>
#include <pgl/memory.h>
#include <stddef.h>
#include <stdlib.h>

#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MACOS
CStr shader_vertex_header = cstr_from_lit("#version 330 core\n");
CStr shader_fragment_header = cstr_from_lit("#version 330 core\n");
#else
CStr shader_vertex_header = cstr_from_lit("#version 300 es\n"
                                          "precision highp float;\n");
CStr shader_fragment_header = cstr_from_lit("#version 300 es\n"
                                            "precision mediump float;\n");
#endif

static UIGraphic ui_graphic = { 0 };

// 0 when failed
GLuint shader_program_create(Scratch *scratch, CStr vert_src, CStr frag_src, StrBuf *log_out) {
  GLint success;

  // Vertex Shader
  GLuint        vertex = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *vShaderCode[2] = { shader_vertex_header.value, vert_src.value };
  glShaderSource(vertex, 2, vShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    CStr  typeStr = cstr_from_lit("Vertex Shader: ");
    GLint infoLength;
    glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);
    strbuf_reserve(scratch, log_out, infoLength + typeStr.size);
    strbuf_append(scratch, log_out, strview_from_cstr(typeStr));
    glGetShaderInfoLog(vertex, infoLength, NULL, log_out->value + log_out->size);
    log_out->size += infoLength;
    return 0;
  }
  // Fragment Shader
  GLuint        fragment = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar *fShaderCode[2] = { shader_fragment_header.value, frag_src.value };
  glShaderSource(fragment, 2, fShaderCode, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    CStr  typeStr = cstr_from_lit("Fragment Shader: ");
    GLint infoLength;
    glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);
    strbuf_reserve(scratch, log_out, infoLength + typeStr.size);
    strbuf_append(scratch, log_out, strview_from_cstr(typeStr));
    glGetShaderInfoLog(fragment, infoLength, NULL, log_out->value + log_out->size);
    log_out->size += infoLength;
    return 0;
  }
  // Shader Program
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    CStr  typeStr = cstr_from_lit("Shader Program: ");
    GLint infoLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
    strbuf_reserve(scratch, log_out, infoLength + typeStr.size);
    glGetProgramInfoLog(program, infoLength, NULL, log_out->value + log_out->size);
    log_out->size += infoLength;
    return 0;
  }
  // Clean up
  glDetachShader(program, vertex);
  glDetachShader(program, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return program;
}

static CStr vsRect = GLSL(
  layout(location = 0) in vec4 c2v_rect;
  layout(location = 1) in vec4 c2v_rectTexture;
  layout(location = 2) in vec4 c2v_color0;
  layout(location = 3) in vec4 c2v_color1;
  layout(location = 4) in vec4 c2v_color2;
  layout(location = 5) in vec4 c2v_color3;
  layout(location = 6) in vec4 c2v_cornerRadii;
  layout(location = 7) in vec4 c2v_style;  // x: border_thickness_px, y: softness_px, z: shearX, w: shearY
  layout(location = 8) in vec4 c2v_style2; // x: no tex | tex | sdf, y: angle_clip

  out vec2 v2p_sdf_sample_pos;
  // out vec2 v2p_texcoord_pct;
  out vec2  v2p_rect_half_size_px;
  out vec4  v2p_tint;
  out float v2p_corner_radius;
  out float v2p_border_thickness;
  out float v2p_softness;
  out float v2p_omit_texture;

  // uniform sampler2D u_tex_color;
  uniform vec2 u_viewportSizePixel;

  void main(void) {
    // constants
    vec2 vertices[] = vec2[](vec2(-1.f, -1.f), vec2(-1.f, +1.f), vec2(+1.f, -1.f), vec2(+1.f, +1.f));

    // unpack shears
    float shearsX[] = float[](0.f, c2v_style.z, 0.f, c2v_style.z);
    float shearsY[] = float[](0.f, 0.f, c2v_style.w, c2v_style.w);

    // find dst position
    vec2 dst_half_size = (c2v_rect.zw - c2v_rect.xy) / 2.f;
    vec2 dst_center = (c2v_rect.zw + c2v_rect.xy) / 2.f;
    vec2 dst_position = vertices[gl_VertexID] * dst_half_size + dst_center;
    dst_position.x += shearsX[gl_VertexID];
    dst_position.y += shearsY[gl_VertexID];

    // find src position
    vec2 src_half_size = (c2v_rectTexture.zw - c2v_rectTexture.xy) / 2.f;
    vec2 src_center = (c2v_rectTexture.zw + c2v_rectTexture.xy) / 2.f;
    vec2 src_position = vertices[gl_VertexID] * src_half_size + src_center;

    // find color
    vec4 colors[] = vec4[](c2v_color0, c2v_color1, c2v_color2, c2v_color3);
    vec4 color = colors[gl_VertexID];

    // find corner radius
    float corner_radius = c2v_cornerRadii[gl_VertexID];

    // fill outputs
    vec2 dst_verts_pct = vec2(((gl_VertexID >> 1) != 1) ? 1.f : 0.f,
                              ((gl_VertexID & 1) != 0) ? 0.f : 1.f);
    // ivec2 u_tex_color_size_i = textureSize(u_tex_color, 0);
    // vec2 u_tex_color_size = vec2(float(u_tex_color_size_i.x), float(u_tex_color_size_i.y));

    gl_Position = vec4(2.f * dst_position.x / u_viewportSizePixel.x - 1.f,
                       2.f * (1.f - dst_position.y / u_viewportSizePixel.y) - 1.f,
                       0.f,
                       1.f);
    v2p_sdf_sample_pos = (2.f * dst_verts_pct - 1.f) * dst_half_size;
    // v2p_texcoord_pct      = src_position / u_tex_color_size;
    v2p_rect_half_size_px = dst_half_size;
    v2p_tint = color;
    v2p_corner_radius = corner_radius;
    v2p_border_thickness = c2v_style.x;
    v2p_softness = c2v_style.y;
    v2p_omit_texture = c2v_style.z;
  });

static CStr fsRect = GLSL(
  in vec2 v2p_sdf_sample_pos;
  // in vec2 v2p_texcoord_pct;
  in vec2  v2p_rect_half_size_px;
  in vec4  v2p_tint;
  in float v2p_corner_radius;
  in float v2p_border_thickness;
  in float v2p_softness;
  in float v2p_omit_texture;

  out vec4 final_color;

  // uniform sampler2D u_tex_color;
  // uniform mat4 u_texture_sample_channel_map;

  float rect_sdf(vec2 sample_pos, vec2 rect_half_size, float r) {
    return length(max(abs(sample_pos) - rect_half_size + r, 0.f)) - r;
  }

  void main(void) {
    // sample texture
    vec4 albedo_sample = vec4(1.f, 1.f, 1.f, 1.f);
    // if(v2p_omit_texture < 1)
    // {
    //   albedo_sample = u_texture_sample_channel_map * texture(u_tex_color, v2p_texcoord_pct);
    //   albedo_sample = linear_from_srgba(albedo_sample);
    // }

    // sample for borders
    float border_sdf_t = 1.f;
    if (v2p_border_thickness > 0.f) {
      float border_sdf_s = rect_sdf(v2p_sdf_sample_pos,
                                    v2p_rect_half_size_px - vec2(v2p_softness * 2.f, v2p_softness * 2.f) - v2p_border_thickness,
                                    max(v2p_corner_radius - v2p_border_thickness, 0.f));
      border_sdf_t = smoothstep(0.f, 2.f * v2p_softness, border_sdf_s);
    }
    if (border_sdf_t < 0.001f) {
      discard;
    }

    // sample for corners
    float corner_sdf_t = 1.f;
    if (v2p_corner_radius > 0.f || v2p_softness > 0.75f) {
      float corner_sdf_s = rect_sdf(v2p_sdf_sample_pos,
                                    v2p_rect_half_size_px - vec2(v2p_softness * 2.f, v2p_softness * 2.f),
                                    v2p_corner_radius);
      corner_sdf_t = 1.f - smoothstep(0.f, 2.f * v2p_softness, corner_sdf_s);
    }

    // form+return final color
    final_color = albedo_sample;
    final_color *= v2p_tint;
    final_color.a *= corner_sdf_t;
    final_color.a *= border_sdf_t;
  });

static void s_create_rect_graphic() {
  ScratchMarker m = scratch_begin(0);
  StrBuf        log = { 0 };
  ui_graphic.rect.shader_program = shader_program_create(m.scratch, vsRect, fsRect, &log);
  if (!ui_graphic.rect.shader_program) {
    log_error("Rect graphic: %s", log.value);
    exit(1);
  }

  glGenVertexArrays(1, &ui_graphic.rect.vao);
  glBindVertexArray(ui_graphic.rect.vao);

  glGenBuffers(1, &ui_graphic.rect.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, ui_graphic.rect.vbo);

  glVertexAttribDivisor(0, 1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, rect));
  glEnableVertexAttribArray(0);

  glVertexAttribDivisor(1, 1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, uvs));
  glEnableVertexAttribArray(1);

  glVertexAttribDivisor(2, 1);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, colors[0]));
  glEnableVertexAttribArray(2);

  glVertexAttribDivisor(3, 1);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, colors[1]));
  glEnableVertexAttribArray(3);

  glVertexAttribDivisor(4, 1);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, colors[2]));
  glEnableVertexAttribArray(4);

  glVertexAttribDivisor(5, 1);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, colors[3]));
  glEnableVertexAttribArray(5);

  glVertexAttribDivisor(6, 1);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, radii));
  glEnableVertexAttribArray(6);

  glVertexAttribDivisor(7, 1);
  glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance), (void *)offsetof(RectInstance, style));
  glEnableVertexAttribArray(7);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  scratch_end(m);
}

static void s_draw_rect_graphic() {
  RectInstance test = {
    { 100, 100, 300, 300 },
    { 0, 0, 0, 0 },
    { { 1, 0, 1, 1 },
      { 1, 0, 0, 1 },
      { 0, 1, 0, 1 },
      { 0, 0, 1, 1 } },
    { 50, 50, 50, 0 },
    { 10, 1.0, 0, -200 }
  };
  dynamic_block_push(&ui_graphic.rect.buffer, &test, RectInstance);

  glUseProgram(ui_graphic.rect.shader_program);
  glBindVertexArray(ui_graphic.rect.vao);
  glBindBuffer(GL_ARRAY_BUFFER, ui_graphic.rect.vbo);

  glBufferData(GL_ARRAY_BUFFER, ui_graphic.rect.buffer.size, ui_graphic.rect.buffer.data, GL_STREAM_DRAW);
  Vec2I32 window_size = window_get_size();
  glUniform2f(glGetUniformLocation(ui_graphic.rect.shader_program, "u_viewportSizePixel"), window_size.x, window_size.y);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ui_graphic.rect.buffer.size / sizeof(RectInstance));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);

  dynamic_block_trim_to(&ui_graphic.rect.buffer, 0);
}

/*
  Rect:
        0--------------> x
        |
        |   xy----o
        |   |     |
        |   o----zw
        V
        Y
  NDC:
                  Y
                  ^
                  |
               xy-|--o
          -----|--0--|----->x
               o--|-zw
                  |
*/

static CStr vsDualKawase = GLSL(
  layout(location = 0) in vec4 c2v_rect;

  out vec2 v2p_texCoord;

  uniform vec2 u_viewportSizePixel;

  void main(void) {
    // construct our rect from 2 corner point
    vec2 vertices[] = vec2[](vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, 1));
    vec2 rect_size = c2v_rect.zw - c2v_rect.xy;
    vec2 vertex_pos_px = c2v_rect.xy + vertices[gl_VertexID] * rect_size;
    // to NDC
    vec2 ndc_pos = (vertex_pos_px / u_viewportSizePixel) * 2.0 - 1.0;
    ndc_pos.y = -ndc_pos.y;
    gl_Position = vec4(ndc_pos, 0.0, 1.0);
    v2p_texCoord = vertex_pos_px / u_viewportSizePixel;
  });

static CStr fsDualKawaseDownsample = GLSL(
  in vec2  v2p_texCoord;
  out vec4 fragColor;

  uniform sampler2D u_texture;
  // CPU sends: vec2(1.0 / srcWidth, 1.0 / srcHeight)
  uniform vec2 u_texelSize;

  void main() {
    // We want to sample strictly between pixel centers to get the
    // hardware average of the 4 surrounding pixels.
    // 0.5 * texelSize puts us right on the corner of the pixels.
    vec2 halfPixel = u_texelSize * 0.5;

    vec4 sum = texture(u_texture, v2p_texCoord) * 4.0;
    sum += texture(u_texture, v2p_texCoord - halfPixel);
    sum += texture(u_texture, v2p_texCoord + halfPixel);
    sum += texture(u_texture, v2p_texCoord + vec2(halfPixel.x, -halfPixel.y));
    sum += texture(u_texture, v2p_texCoord - vec2(halfPixel.x, -halfPixel.y));

    fragColor = sum * 0.125;
  });

static CStr fsDualKawaseUpsample = GLSL(
  in vec2  v2p_texCoord;
  out vec4 fragColor;

  uniform sampler2D u_texture;
  uniform vec2      u_texelSize; // Pass 1.0 / ScreenSize

  void main() {
    // The "halfpixel" in your image usually refers to half of a texel size
    // if using the standard Kawase offset logic.
    vec2 halfpixel = u_texelSize * 0.5;

    vec4 sum = vec4(0.0);

    // 1. Cardinals (Offset by 2.0) - Weight 1.0
    // These are farther away, so they get less weight.
    sum += texture(u_texture, v2p_texCoord + vec2(-u_texelSize.x, 0.0));
    sum += texture(u_texture, v2p_texCoord + vec2(0.0, u_texelSize.y));
    sum += texture(u_texture, v2p_texCoord + vec2(u_texelSize.x, 0.0));
    sum += texture(u_texture, v2p_texCoord + vec2(0.0, -u_texelSize.y));

    // 2. Diagonals (Offset by 1.0) - Weight 2.0
    // These are closer to the center, so they get double the importance.
    sum += texture(u_texture, v2p_texCoord + vec2(-halfpixel.x, halfpixel.y)) * 2.0;
    sum += texture(u_texture, v2p_texCoord + vec2(halfpixel.x, halfpixel.y)) * 2.0;
    sum += texture(u_texture, v2p_texCoord + vec2(halfpixel.x, -halfpixel.y)) * 2.0;
    sum += texture(u_texture, v2p_texCoord + vec2(-halfpixel.x, -halfpixel.y)) * 2.0;

    // Divide by total weight (4*1 + 4*2 = 12)
    fragColor = sum / 12.0;
  });

static void s_create_blur_graphic() {
  ScratchMarker m = scratch_begin(0);
  StrBuf        log = { 0 };
  ui_graphic.blur.downsample_program = shader_program_create(m.scratch,
                                                             vsDualKawase,
                                                             fsDualKawaseDownsample,
                                                             &log);
  if (!ui_graphic.blur.downsample_program) {
    log_error("Blur graphic: %s", log.value);
    exit(1);
  }
  ui_graphic.blur.upsample_program = shader_program_create(m.scratch,
                                                           vsDualKawase,
                                                           fsDualKawaseUpsample,
                                                           &log);
  if (!ui_graphic.blur.upsample_program) {
    log_error("Blur graphic: %s", log.value);
    exit(1);
  }

  glGenVertexArrays(1, &ui_graphic.blur.vao);
  glBindVertexArray(ui_graphic.blur.vao);
  glGenBuffers(1, &ui_graphic.blur.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, ui_graphic.blur.vbo);

  glVertexAttribDivisor(0, 1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
                        sizeof(BlurInstance),
                        (void *)offsetof(BlurInstance, rect));
  glEnableVertexAttribArray(0);

  glVertexAttribDivisor(1, 1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                        sizeof(BlurInstance),
                        (void *)offsetof(BlurInstance, radii));
  glEnableVertexAttribArray(1);

  glGenFramebuffers(2, ui_graphic.blur.fbo);
  glGenTextures(2, ui_graphic.blur.fbo_tex);

  for (int i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, ui_graphic.blur.fbo_tex[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear is REQUIRED for the sampling trick
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, ui_graphic.blur.fbo[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           ui_graphic.blur.fbo_tex[i],
                           0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  scratch_end(m);
}

static void draw_blur_graphic() {
  BlurInstance test = {
    { 100, 100, 300, 300 },
    { 10, 10, 10, 10 }
  };
  dynamic_block_push(&ui_graphic.blur.buffer, &test, BlurInstance);

  int count = ui_graphic.blur.buffer.size / sizeof(BlurInstance);
  if (count == 0) {
    return;
  }
  Vec2I32 window_size = window_get_size();
  int     w = window_size.x;
  int     h = window_size.y;
  if (w == 0 || h == 0) {
    return;
  }

  // set the size of the texture
  for (int i = 0; i < 2; ++i) {
    glBindTexture(GL_TEXTURE_2D, ui_graphic.blur.fbo_tex[i]);
    glTexImage2D(GL_TEXTURE_2D, 0,
                 GL_RGBA,
                 w, h, 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL);
  }

  // Copy Screen (Read: 0) -> Texture 0 (Draw: FBO 0)
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ui_graphic.blur.fbo[0]);
  glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glUseProgram(ui_graphic.blur.shader_program);
  glBindVertexArray(ui_graphic.blur.vao);
  glBindBuffer(GL_ARRAY_BUFFER, ui_graphic.blur.vbo);
  glBufferData(GL_ARRAY_BUFFER, ui_graphic.blur.buffer.size, ui_graphic.blur.buffer.data, GL_STREAM_DRAW);

  GLint loc_viewport = glGetUniformLocation(ui_graphic.blur.shader_program, "u_viewportSizePixel");
  GLint loc_dir = glGetUniformLocation(ui_graphic.blur.shader_program, "u_direction");

  glUniform2f(loc_viewport, app.window.sizePixel.x, app.window.sizePixel.y);

  // horizontal pass
  glBindFramebuffer(GL_FRAMEBUFFER, ui_graphic.blur.fbo[1]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ui_graphic.blur.fbo_tex[0]);

  glUniform2f(loc_dir, 1.f, 0.f); // Horizontal Direction
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

  // vertical pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, ui_graphic.blur.fbo_tex[1]);

  glUniform2f(loc_dir, 0.f, 1.f); // Vertical Direction
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);

  dynamic_block_trim_to(&ui_graphic.blur.buffer, 0);
}

void create_ui_graphic() {
  s_create_rect_graphic();
  // s_create_blur_graphic();
}

void draw_ui_graphic() {
  s_draw_rect_graphic();
  // draw_blur_graphic();
}
