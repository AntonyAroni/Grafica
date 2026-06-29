#ifndef GLM_HPP
#define GLM_HPP

#include <cmath>

namespace glm {

struct vec2 {
    float x;
    float y;

    vec2() : x(0.0f), y(0.0f) {}
    vec2(float x, float y) : x(x), y(y) {}
    explicit vec2(float s) : x(s), y(s) {}

    vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }
    vec2 operator-(const vec2& v) const { return vec2(x - v.x, y - v.y); }
    vec2 operator*(float s) const { return vec2(x * s, y * s); }
    vec2 operator/(float s) const { return vec2(x / s, y / s); }

    vec2& operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
    vec2& operator-=(const vec2& v) { x -= v.x; y -= v.y; return *this; }
    vec2& operator*=(float s) { x *= s; y *= s; return *this; }
    vec2& operator/=(float s) { x /= s; y /= s; return *this; }

    vec2 operator-() const { return vec2(-x, -y); }
};

inline vec2 operator*(float s, const vec2& v) { return vec2(v.x * s, v.y * s); }

inline float length(const vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float distance(const vec2& a, const vec2& b) {
    return length(a - b);
}

inline vec2 normalize(const vec2& v) {
    float len = length(v);
    if (len > 0.0001f) {
        return v / len;
    }
    return vec2(0.0f, 0.0f);
}

inline float dot(const vec2& a, const vec2& b) {
    return a.x * b.x + a.y * b.y;
}

} // namespace glm

#endif
