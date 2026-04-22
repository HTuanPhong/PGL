#include <math.h>
#include <pgl/math.h>

f32 sin_f32(f32 rad) {
  return sinf(rad);
}

f32 cos_f32(f32 rad) {
  return cosf(rad);
}

f32 TanF(f32 rad) {
  return tanf(rad);
}

f32 acos_f32(f32 arg) {
  return acosf(arg);
}

f32 sqrt_f32(f32 arg) {
  return sqrtf(arg);
}

f32 lerp(f32 A, f32 T, f32 B) {
  return (1.0f - T) * A + T * B;
}

f32 clamp_f32(f32 min, f32 val, f32 max) {
  return (val < min) ? min : ((val > max) ? max : val);
}

Vec2F32 add_vec2_f32(Vec2F32 left, Vec2F32 right) {
  Vec2F32 result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  return result;
}

Vec3F32 add_vec3_f32(Vec3F32 left, Vec3F32 right) {
  Vec3F32 result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  return result;
}

Vec4F32 add_vec4_f32(Vec4F32 left, Vec4F32 right) {
  Vec4F32 result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  result.w = left.w + right.w;
  return result;
}

Vec2F32 sub_vec2_f32(Vec2F32 left, Vec2F32 right) {
  Vec2F32 result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  return result;
}

Vec3F32 sub_vec3_f32(Vec3F32 left, Vec3F32 right) {
  Vec3F32 result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  return result;
}

Vec4F32 sub_vec4_f32(Vec4F32 left, Vec4F32 right) {
  Vec4F32 result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  result.w = left.w - right.w;
  return result;
}
