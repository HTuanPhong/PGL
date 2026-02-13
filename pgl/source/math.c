#include <math.h>
#include <pgl/math.h>

float SinF(float rad) {
  return sinf(rad);
}

float CosF(float rad) {
  return cosf(rad);
}

float TanF(float rad) {
  return tanf(rad);
}

float ACosF(float arg) {
  return acosf(arg);
}

float SqrtF(float arg) {
  return sqrtf(arg);
}

float Lerp(float A, float T, float B) {
  return (1.0f - T) * A + T * B;
}

float Clamp(float min, float val, float max) {
  return (val < min) ? min : ((val > max) ? max : val);
}

Vec2F AddVec2F(Vec2F left, Vec2F right) {
  Vec2F result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  return result;
}

Vec3F AddVec3F(Vec3F left, Vec3F right) {
  Vec3F result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  return result;
}

Vec4F AddVec4F(Vec4F left, Vec4F right) {
  Vec4F result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  result.w = left.w + right.w;
  return result;
}

Vec2F SubVec2F(Vec2F left, Vec2F right) {
  Vec2F result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  return result;
}

Vec3F SubVec3F(Vec3F left, Vec3F right) {
  Vec3F result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  return result;
}

Vec4F SubVec4F(Vec4F left, Vec4F right) {
  Vec4F result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  result.w = left.w - right.w;
  return result;
}
