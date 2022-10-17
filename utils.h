#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;

struct vec2 { 
	float x, y; 
};

sf::Vector2f to_sfml(vec2 v);
vec2 to_cm(sf::Vector2f v);

float randf();
vec2 rand_vec2();

bool operator==(vec2 a, vec2 b);
bool operator!=(vec2 a, vec2 b);
vec2 operator+(vec2 a, vec2 b);
vec2 operator+(vec2 a, float b);
vec2 operator+(float a, vec2 b);
vec2& operator+=(vec2& a, vec2 b);
vec2& operator+=(vec2& a, float b);

vec2 operator-(vec2 a, float b);
vec2 operator-(float a, vec2 b);
vec2 operator-(vec2 b);
vec2 operator-(vec2 a, vec2 b);
vec2& operator-=(vec2& a, vec2 b);
vec2& operator-=(vec2& a, float b);

vec2 operator*(vec2 a, vec2 b);
vec2 operator*(vec2 a, float b);
vec2 operator*(float a, vec2 b);
vec2& operator*=(vec2& a, vec2 b);
vec2& operator*=(vec2& a, float b);

vec2 operator/(vec2 a, vec2 b);
vec2 operator/(vec2 a, float b);
vec2 operator/(float a, vec2 b);
vec2& operator/=(vec2& a, vec2 b);
vec2& operator/=(vec2& a, float b);

float dot(vec2 a, vec2 b);
float len2(vec2 v);
float len(vec2 v);
vec2 lrot(vec2 a);
float cross(vec2 a, vec2 b);
vec2 lerp(vec2 a, vec2 b, float t);
