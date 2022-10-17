#include "utils.h"
#include <cmath>

sf::Vector2f to_sfml(vec2 v) { return {v.x, v.y}; }
vec2 to_cm(sf::Vector2f v) { return {v.x, v.y}; }

bool operator==(vec2 a, vec2 b) { return a.x == b.x && a.y == b.y; }
bool operator!=(vec2 a, vec2 b) { return a.x != b.x || a.y != b.y; }

float randf() { return (float)rand() / RAND_MAX; }
vec2 rand_vec2() { return {randf(), randf()}; }

vec2 operator+(vec2 a, vec2 b) { return {a.x + b.x, a.y + b.y}; }
vec2 operator+(vec2 a, float b) { return {a.x + b, a.y + b}; }
vec2 operator+(float a, vec2 b) { return b + a; }
vec2& operator+=(vec2& a, vec2 b) { return a = a + b; }
vec2& operator+=(vec2& a, float b) { return a = a + b; }

vec2 operator-(vec2 a, float b) { return {a.x - b, a.y - b}; }
vec2 operator-(float a, vec2 b) { return {a - b.x, a - b.y}; }
vec2 operator-(vec2 b) { return {-b.x, -b.y}; }
vec2 operator-(vec2 a, vec2 b) { return {a.x - b.x, a.y - b.y}; }
vec2& operator-=(vec2& a, vec2 b) { return a = a - b; }
vec2& operator-=(vec2& a, float b) { return a = a - b; }

vec2 operator*(vec2 a, vec2 b) { return {a.x * b.x, a.y * b.y}; }
vec2 operator*(vec2 a, float b) { return {a.x * b, a.y * b}; }
vec2 operator*(float a, vec2 b) { return {a * b.x, a * b.y}; }
vec2& operator*=(vec2& a, vec2 b) { return a = a * b; }
vec2& operator*=(vec2& a, float b) { return a = a * b; }

vec2 operator/(vec2 a, vec2 b) { return {a.x / b.x, a.y / b.y}; }
vec2 operator/(vec2 a, float b) { return {a.x / b, a.y / b}; }
vec2 operator/(float a, vec2 b) { return {a / b.x, a / b.y}; }
vec2& operator/=(vec2& a, vec2 b) { return a = a / b; }
vec2& operator/=(vec2& a, float b) { return a = a / b; }

float dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float len2(vec2 v) { return v.x * v.x + v.y * v.y; }
float len(vec2 v) { return sqrtf(len2(v)); }
vec2 lrot(vec2 a) { return {-a.y, a.x}; }
float cross(vec2 a, vec2 b) { return a.x * b.y - a.y * b.x; }
vec2 lerp(vec2 a, vec2 b, float t) { return a + t * (b - a); }