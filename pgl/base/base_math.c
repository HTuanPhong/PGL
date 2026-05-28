FUNCTION u64 muldiv_u64(u64 v, u64 n, u64 d) {
  u64 q = v / d;
  u64 r = v % d;
  return q * n + (r * n) / d;
}

FUNCTION f32 abs_f32(f32 n) {
  return fabsf(n);
}
FUNCTION f32 mod_f32(f32 x, f32 y) {
  return fmodf(x, y);
}
FUNCTION f32 log_f32(f32 n) {
  return logf(n);
}
FUNCTION f32 log10_f32(f32 n) {
  return log10f(n);
}
FUNCTION f32 pow_f32(f32 base, f32 exponent) {
  return powf(base, exponent);
}
FUNCTION f32 sqrt_f32(f32 n) {
  return sqrtf(n);
}
FUNCTION f32 cbrt_f32(f32 n) {
  return cbrtf(n);
}
FUNCTION f32 sin_f32(f32 rad) {
  return sinf(rad);
}
FUNCTION f32 cos_f32(f32 rad) {
  return cosf(rad);
}
FUNCTION f32 tan_f32(f32 rad) {
  return tanf(rad);
}
FUNCTION f32 asin_f32(f32 n) {
  return asinf(n);
}
FUNCTION f32 acos_f32(f32 n) {
  return acosf(n);
}
FUNCTION f32 atan_f32(f32 y, f32 x) {
  return atan2f(y, x);
}
FUNCTION f32 ceil_f32(f32 n) {
  return ceilf(n);
}
FUNCTION f32 floor_f32(f32 n) {
  return floorf(n);
}
FUNCTION f32 trunc_f32(f32 n) {
  return truncf(n);
}
FUNCTION f32 round_f32(f32 n) {
  return roundf(n);
}
FUNCTION f32 lerp_f32(f32 A, f32 T, f32 B) {
  return (1.0f - T) * A + T * B;
}

FUNCTION f64 abs_f64(f64 n) {
  return fabs(n);
}
FUNCTION f64 mod_f64(f64 x, f64 y) {
  return fmod(x, y);
}
FUNCTION f64 log_f64(f64 n) {
  return log(n);
}
FUNCTION f64 log10_f64(f64 n) {
  return log10(n);
}
FUNCTION f64 pow_f64(f64 base, f64 exponent) {
  return pow(base, exponent);
}
FUNCTION f64 sqrt_f64(f64 n) {
  return sqrt(n);
}
FUNCTION f64 cbrt_f64(f64 n) {
  return cbrt(n);
}
FUNCTION f64 sin_f64(f64 rad) {
  return sin(rad);
}
FUNCTION f64 cos_f64(f64 rad) {
  return cos(rad);
}
FUNCTION f64 tan_f64(f64 rad) {
  return tan(rad);
}
FUNCTION f64 asin_f64(f64 n) {
  return asin(n);
}
FUNCTION f64 acos_f64(f64 n) {
  return acos(n);
}
FUNCTION f64 atan_f64(f64 y, f64 x) {
  return atan2(y, x);
}
FUNCTION f64 ceil_f64(f64 n) {
  return ceil(n);
}
FUNCTION f64 floor_f64(f64 n) {
  return floor(n);
}
FUNCTION f64 trunc_f64(f64 n) {
  return trunc(n);
}
FUNCTION f64 round_f64(f64 n) {
  return round(n);
}
FUNCTION f64 lerp_f64(f64 A, f64 T, f64 B) {
  return (1.0 - T) * A + T * B;
}

FUNCTION Vec2F32
add_vec2_f32(Vec2F32 left, Vec2F32 right) {
  Vec2F32 result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  return result;
}

FUNCTION Vec3F32
add_vec3_f32(Vec3F32 left, Vec3F32 right) {
  Vec3F32 result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  return result;
}

FUNCTION Vec4F32
add_vec4_f32(Vec4F32 left, Vec4F32 right) {
  Vec4F32 result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  result.w = left.w + right.w;
  return result;
}

FUNCTION Vec2F32
sub_vec2_f32(Vec2F32 left, Vec2F32 right) {
  Vec2F32 result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  return result;
}

FUNCTION Vec3F32
sub_vec3_f32(Vec3F32 left, Vec3F32 right) {
  Vec3F32 result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  return result;
}

FUNCTION Vec4F32
sub_vec4_f32(Vec4F32 left, Vec4F32 right) {
  Vec4F32 result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  result.w = left.w - right.w;
  return result;
}
