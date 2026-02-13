#ifndef PGL_MATH_H
#define PGL_MATH_H

#define PI 3.14159265358979323846f

// Vector 2
typedef union Vec2F {
  struct
  {
    float x;
    float y;
  };
  float values[2];
} Vec2F;

typedef union Vec2I {
  struct
  {
    int x;
    int y;
  };
  int values[2];
} Vec2I;

// Vector 3

typedef union Vec3F {
  struct
  {
    float x;
    float y;
    float z;
  };
  struct
  {
    Vec2F xy;
    float _z0;
  };
  struct
  {
    float _x0;
    Vec2F yz;
  };
  float values[3];
} Vec3F;

typedef union Vec3I {
  struct
  {
    int x;
    int y;
    int z;
  };
  struct
  {
    Vec2I xy;
    int   _z0;
  };
  struct
  {
    int   _x0;
    Vec2I yz;
  };
  int values[3];
} Vec3I;

// Vector 4

typedef union Vec4F {
  struct
  {
    float x;
    float y;
    float z;
    float w;
  };
  struct
  {
    Vec2F xy;
    Vec2F zw;
  };
  struct
  {
    Vec3F xyz;
    float _z0;
  };
  struct
  {
    float _x0;
    Vec3F yzw;
  };
  float values[4];
} Vec4F;

// matrix 3x3

typedef union Mat3x3F {
  float values[3][3];
  Vec3F columns[3];
} Mat3x3F;

// matrix 4x4

typedef union Mat4x4F {
  float values[4][4];
  Vec4F columns[4];
} Mat4x4F;

typedef union Quat Quat;
union Quat {
  struct
  {
    float x;
    float y;
    float z;
    float w;
  };
  struct
  {
    Vec3F xyz;
    float _w0;
  };
  float values[4];
};

float SinF(float rad);
float CosF(float rad);
float TanF(float rad);
float ACosF(float arg);
float SqrtF(float arg);
float Lerp(float A, float T, float B);
float Clamp(float min, float val, float max);
Vec2F AddVec2F(Vec2F left, Vec2F right);
Vec3F AddVec3F(Vec3F left, Vec3F right);
Vec4F AddVec4F(Vec4F left, Vec4F right);
Vec2F AddVec2F(Vec2F left, Vec2F right);
Vec3F AddVec3F(Vec3F left, Vec3F right);
Vec4F AddVec4F(Vec4F left, Vec4F right);

#endif // PGL_MATH_H
