#if !defined(_ELIA_2D_ENGINE_H_)
#define _ELIA_2D_ENGINE_H_

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Utils/Math.h"
#include "Utils/FixedString.hpp"
#include "Utils/Input.hpp"

namespace Elia2D
{
    using Texture = uint32_t;
    enum Result
    {
        Success, Failure
    };
    
    v2f PixelToNormalized(v2 aPoint);

    Result Start(
        const FixedString256& anAppName,
        void (*anUpdateCallback)(float),
        void (*anInitCallback)() = 0,
        uint16_t aWidth = 1280U,
        uint16_t aHeight = 720U,
        Color aColor = { 64, 72, 92 });

    void DrawTriangle(v2 p1, v2 p2, v2 p3, Color aColor = Color());
    void DrawRect(v2 aPosition, v2 aSize, Color aColor = Color());
    void DrawCircle(v2 aPosition, int aRadius, Color aColor = Color());
    void DrawTexturedQuad(Texture aTexture, v2 aPosition, v2 aSize, Color aColor = Color(), v4f aUV = {0,0,1,1});

    void OnWindowResize(void (*aCallback)(uint16_t, uint16_t));
    void OnDragDrop(void (*aCallback)(const char**, size_t));
    void SetWindowTitle(const char* const aTitle);
    void OnInput(void (*aCallback)(int, int));
    void SetClearColor(Color aColor);
    v2f GetMousePosition();

    Texture CreateTexture(const char* const aPath, v2* aSizeTarget = 0);
    void ReturnTexture(Texture aTexture);

    void SetCameraPosition(v2f aPosition);
    v2f GetCameraPosition();
}

#endif // _ELIA_2D_ENGINE_H_
