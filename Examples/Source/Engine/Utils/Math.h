#if !defined(_ELIA2D_MATH_H_)
#define _ELIA2D_MATH_H_

#pragma once

#include <stdint.h>
#include <math.h>

namespace Elia2D
{
    constexpr double pi = 3.1415926535897932384626433832795028841971693993751058209749445923;
	constexpr double tau = 2.0 * pi;
	constexpr double e = 2.71828182845904523536028747135266249775724709369995957496696762774;
	constexpr double sqrt2 = 1.4142135623730950488016887242096980785696718753769480731766797379;
	constexpr double sqrt3 = 1.7320508075688772935274463415058723669428052538103806280558069794;
	constexpr double sqrt5 = 2.2360679774997896964091736687312762354406183596115257242708972454;
	constexpr double phi = .5 * (1 + sqrt5);
	constexpr float infinity = (float)(1e+308 * 1e+308);

    struct v2
    {
        int x{}, y{};
    };

    struct v3
    {
        int x{}, y{}, z{};
    };

    struct v2f
    {
        float x{}, y{};
    };

    struct v3f
    {
        float x{}, y{}, z{};
    };
    struct v4f
    {
        float x{}, y{}, z{}, w{};
    };

    struct Color
    {
        uint8_t r{255}, g{255}, b{255};
    };

    inline v4f Normalize(Color col)
    {
        return { float(col.r) / 256.0f, float(col.g) / 256.0f, float(col.b) / 256.0f, 1.0f };
    }

    inline float Len(v2f v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    inline v2f Normalize(v2f v)
    {
        const float len = Len(v);
        if (!len) return { .0f, .0f };
        return { v.x / len, v.y / len };
    }

    inline float Abs(float f)
    {
        return ((f < 0.0f) ? f * -1.0f : f);
    }

    inline int Max(int a, int b)
    {
        return ((a > b) ? a : b);
    }

    inline float Maxf(float a, float b)
    {
        return ((a > b) ? a : b);
    }

    inline int Min(int a, int b)
    {
        return ((a < b) ? a : b);
    }

    inline uint32_t Minu(uint32_t a, uint32_t b)
    {
        return ((a < b) ? a : b);
    }

    inline float Minf(float a, float b)
    {
        return ((a < b) ? a : b);
    }

    inline float AbsMinf(float a, float b)
    {
        return ((Abs(a) < Abs(b)) ? a : b);
    }

    inline float Dot(v2f a, v2f b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline v2f Add(v2f a, v2f b)
    {
        return { a.x + b.x, a.y + b.y };
    }

    inline v2f Sub(v2f a, v2f b)
    {
        return { a.x - b.x, a.y - b.y };
    }

    inline v2f Mul(v2f a, float b)
    {
        return { a.x * b, a.y * b };
    }

    inline v2f Div(v2f a, v2f b)
    {
        return { a.x / b.x, a.y / b.y };
    }

    inline v2f Div(v2f a, float b)
    {
        return { a.x / b, a.y / b };
    }

    inline v2 Div(v2 a, int b)
    {
        return { a.x / b, a.y / b };
    }

    inline v2 Convertv2f_v2(v2f v)
    {
        return { int(v.x), int(v.y) };
    }

    inline v2f Convertv2_v2f(v2 v)
    {
        return { float(v.x), float(v.y) };
    }

    inline float Lerp(float a, float b, float v)
    {
        return a + (b - a) * v;
    }
    inline int Lerp(int a, int b, float v)
    {
        float _a = float(a), _b = float(b);
        return int(_a + (_b - _a) * v);
    }
    inline uint32_t Lerp(uint32_t a, uint32_t b, float v)
    {
        float _a = float(a), _b = float(b);
        return uint32_t(_a + (_b - _a) * v);
    }

    inline bool Eq(v2f a, v2f b)
    {
        return a.x == b.x && a.y == b.y;
    }
}

#endif // _ELIA2D_MATH_H_
