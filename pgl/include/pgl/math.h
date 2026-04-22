#ifndef PGL_MATH_H
#define PGL_MATH_H

#include <pgl/defines.h>

// wrap all math.h functions because we might simd them

#define PI 3.14159265358979323846f

// Vector 2
typedef union Vec2F32 {
  struct
  {
    f32 x;
    f32 y;
  };
  f32 values[2];
} Vec2F32;

typedef union Vec2I32 {
  struct
  {
    i32 x;
    i32 y;
  };
  i32 values[2];
} Vec2I32;

// Vector 3

typedef union Vec3F32 {
  struct
  {
    f32 x;
    f32 y;
    f32 z;
  };
  struct
  {
    Vec2F32 xy;
    f32     _z0;
  };
  struct
  {
    f32     _x0;
    Vec2F32 yz;
  };
  f32 values[3];
} Vec3F32;

typedef union Vec3I32 {
  struct
  {
    i32 x;
    i32 y;
    i32 z;
  };
  struct
  {
    Vec2I32 xy;
    i32     _z0;
  };
  struct
  {
    i32     _x0;
    Vec2I32 yz;
  };
  i32 values[3];
} Vec3I32;

// Vector 4

typedef union Vec4F32 {
  struct
  {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
  };
  struct
  {
    Vec2F32 xy;
    Vec2F32 zw;
  };
  struct
  {
    Vec3F32 xyz;
    f32     _z0;
  };
  struct
  {
    f32     _x0;
    Vec3F32 yzw;
  };
  f32 values[4];
} Vec4F32;

// matrix 3x3

typedef union Mat3F32 {
  f32     values[3][3];
  Vec3F32 columns[3];
} Mat3F32;

// matrix 4x4

typedef union Mat4F32 {
  f32     values[4][4];
  Vec4F32 columns[4];
} Mat4F32;

f32 sin_f32(f32 rad);
f32 cos_f32(f32 rad);
f32 tan_f32(f32 rad);
f32 acos_f32(f32 arg);
f32 sqrt_f32(f32 arg);
f32 lerp_f32(f32 A, f32 T, f32 B);

Vec2F32 add_vec2_f32(Vec2F32 left, Vec2F32 right);
Vec3F32 add_vec3_f32(Vec3F32 left, Vec3F32 right);
Vec4F32 add_vec4_f32(Vec4F32 left, Vec4F32 right);
Vec2F32 sub_vec2_f32(Vec2F32 left, Vec2F32 right);
Vec3F32 sub_vec3_f32(Vec3F32 left, Vec3F32 right);
Vec4F32 sub_vec4_f32(Vec4F32 left, Vec4F32 right);

#endif // PGL_MATH_H
