#ifndef BASE_MATH_H
#define BASE_MATH_H

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

// range

// ...

// overflow aware

function u64 muldiv_u64(u64 v, u64 n, u64 d);

// Basic operations
function f32 abs_f32(f32 n);
function f32 mod_f32(f32 x, f32 y);
function f32 log_f32(f32 n);
function f32 log10_f32(f32 n);
function f32 pow_f32(f32 base, f32 exponent);
function f32 sqrt_f32(f32 n);
function f32 cbrt_f32(f32 n);
function f32 sin_f32(f32 rad);
function f32 cos_f32(f32 rad);
function f32 tan_f32(f32 rad);
function f32 asin_f32(f32 n);
function f32 acos_f32(f32 n);
function f32 atan_f32(f32 y, f32 x); // atan2 y/x
function f32 ceil_f32(f32 n);
function f32 floor_f32(f32 n);
function f32 trunc_f32(f32 n);
function f32 round_f32(f32 n);
function f32 lerp_f32(f32 A, f32 T, f32 B);

function f64 abs_f64(f64 n);
function f64 mod_f64(f64 x, f64 y);
function f64 log_f64(f64 n);
function f64 log10_f64(f64 n);
function f64 pow_f64(f64 base, f64 exponent);
function f64 sqrt_f64(f64 n);
function f64 cbrt_f64(f64 n);
function f64 sin_f64(f64 rad);
function f64 cos_f64(f64 rad);
function f64 tan_f64(f64 rad);
function f64 asin_f64(f64 n);
function f64 acos_f64(f64 n);
function f64 atan_f64(f64 y, f64 x); // atan2 y/x
function f64 ceil_f64(f64 n);
function f64 floor_f64(f64 n);
function f64 trunc_f64(f64 n);
function f64 round_f64(f64 n);
function f64 lerp_f64(f64 A, f64 T, f64 B);

function Vec2F32 add_vec2_f32(Vec2F32 left, Vec2F32 right);
function Vec3F32 add_vec3_f32(Vec3F32 left, Vec3F32 right);
function Vec4F32 add_vec4_f32(Vec4F32 left, Vec4F32 right);
function Vec2F32 sub_vec2_f32(Vec2F32 left, Vec2F32 right);
function Vec3F32 sub_vec3_f32(Vec3F32 left, Vec3F32 right);
function Vec4F32 sub_vec4_f32(Vec4F32 left, Vec4F32 right);


#endif
