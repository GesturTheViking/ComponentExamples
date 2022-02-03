#include "Engine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace Elia2D
{  
    struct PrimitiveData
    {
        uint32_t vertexArrayObject = 0U;
        uint32_t vertexBufferObject = 0U;
        uint32_t indexBufferObject = 0U;
    };
    struct CircleData
    {
        uint32_t vertexArrayObject = 0U;
        uint32_t vertexBufferObject = 0U;
        uint32_t shaderProgram = 0U;
    };
    struct TexturedQuadData
    {
        uint32_t vertexArrayObject = 0U;
        uint32_t vertexBufferObject = 0U;
        uint32_t shaderProgram = 0U;
    };

    struct
    {
        uint16_t winWidth = 1280, winHeight = 720;
        float invHalfWinWidth = 1.0f / (.5f * 1280.0f), invHalfWinHeight = 1.0f / (.5f * 720.0f);
        v4f clearColor = { .25f, .28f, .36f, 1.0f };

        uint32_t primShaderProgram = 0U;
        PrimitiveData triData;
        PrimitiveData rectData;
        CircleData circleData;
        TexturedQuadData texturedQuadData;

        void (*windowResizeCallback)(uint16_t, uint16_t) = nullptr;
        void (*dragDropCallback)(const char**, size_t) = nullptr;
        void (*keyCallback)(int, int) = nullptr;
        GLFWwindow* window = nullptr;

        v2f cameraPosition;
    } globals;

    GLFWimage LoadIcon(const FixedString256& aPath, GLFWwindow* const aWindow)
    {
        struct
        {
            char garbage[12];
            int16_t width;
            int16_t height;
            char cabbage[2];
        } tgaHeader;
        FILE* iconFile;
        fopen_s(&iconFile, aPath, "rb");
        GLFWimage icon;
        if (iconFile)
        {
            fread(&tgaHeader, sizeof tgaHeader, 1, iconFile);
            icon.width = int(tgaHeader.width);
            icon.height = int(tgaHeader.height);
            const size_t imageSize = icon.width * icon.height * 4;
            icon.pixels = (uint8_t*)malloc(imageSize);
            fread(icon.pixels, imageSize, 1, iconFile);
            glfwSetWindowIcon(aWindow, 1, &icon);
            fclose(iconFile);
        }
        return icon;
    }

    v2f PixelToNormalized(v2 aPoint)
    {
        v2f n1 = {float(aPoint.x) - globals.cameraPosition.x, float(aPoint.y) - globals.cameraPosition.y};
        n1.x *= globals.invHalfWinWidth; n1.y *= globals.invHalfWinHeight;
        n1.x -= 1.0f; n1.y -= 1.0f;
        n1.y *= -1.0f;
        return n1;
    }

    uint32_t CreateShaderProgram(const char* const aVertShader, const char* const aFragShader);

    Result Start(const FixedString256& anAppName, void (*anUpdateCallback)(float), void (*anInitCallback)(), uint16_t aWidth, uint16_t aHeight, Color aColor)
    {
        Result res;

        /* Init Window */
        {
            GLFWwindow* window;

            if (!glfwInit())
                return Result::Failure;

            globals.winWidth = aWidth;
            globals.winHeight = aHeight;
            globals.invHalfWinWidth = 1.0f / (.5f * float(aWidth));
            globals.invHalfWinHeight = 1.0f / (.5f * float(aHeight));
            globals.clearColor = Normalize(aColor);

            window = glfwCreateWindow(aWidth, aHeight, "Elia2D - " + anAppName, NULL, NULL);
            if (!window)
            {
                glfwTerminate();
                return Result::Failure;
            }
            globals.window = window;

            glfwMakeContextCurrent(window);

            glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height)
            {
                glViewport(0, 0, width, height);

                globals.winWidth = uint16_t(width);
                globals.winHeight = uint16_t(height);
                globals.invHalfWinWidth = 1.0f / (.5f * float(width));
                globals.invHalfWinHeight = 1.0f / (.5f * float(height));

                if (globals.windowResizeCallback)
                {
                    globals.windowResizeCallback(globals.winWidth, globals.winHeight);
                }
            });

            glfwSetKeyCallback(globals.window, [](GLFWwindow* window, int key, int, int action, int)
            {
                if (globals.keyCallback)
                {
                    globals.keyCallback(key, action);
                }
            });
            glfwSetDropCallback(window, [](GLFWwindow* window, int size, const char** pathArr)
            {
                if (globals.dragDropCallback)
                {
                    globals.dragDropCallback(pathArr, size_t(size));
                }
            });
        }


        /* Get OpenGL Function Pointers */
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return Result::Failure;
        }

        /* Init Dear ImGui */
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ImGui::StyleColorsDark();
            
            /* ImGui Style */
            {
                ImGuiStyle* style = &ImGui::GetStyle();
                style->WindowPadding = ImVec2(15, 15);
                style->WindowRounding = 5.0f;
                style->FramePadding = ImVec2(5, 5);
                style->FrameRounding = 4.0f;
                style->ItemSpacing = ImVec2(12, 8);
                style->ItemInnerSpacing = ImVec2(8, 6);
                style->IndentSpacing = 25.0f;
                style->ScrollbarSize = 15.0f;
                style->ScrollbarRounding = 9.0f;
                style->GrabMinSize = 5.0f;
                style->GrabRounding = 3.0f;
                style->Colors[ImGuiCol_Text]                    = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
                style->Colors[ImGuiCol_TextDisabled]            = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                style->Colors[ImGuiCol_WindowBg]                = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                style->Colors[ImGuiCol_PopupBg]                 = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                style->Colors[ImGuiCol_BorderShadow]            = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
                style->Colors[ImGuiCol_FrameBg]                 = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                style->Colors[ImGuiCol_FrameBgActive]           = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                style->Colors[ImGuiCol_TitleBg]                 = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
                style->Colors[ImGuiCol_TitleBgActive]           = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                style->Colors[ImGuiCol_MenuBarBg]               = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                style->Colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                style->Colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                style->Colors[ImGuiCol_CheckMark]               = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                style->Colors[ImGuiCol_SliderGrab]              = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                style->Colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                style->Colors[ImGuiCol_Button]                  = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_ButtonHovered]           = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                style->Colors[ImGuiCol_ButtonActive]            = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                style->Colors[ImGuiCol_Header]                  = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_HeaderHovered]           = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                style->Colors[ImGuiCol_HeaderActive]            = ImVec4(0.6f, 0.58f, 0.62f, 1.00f);
                style->Colors[ImGuiCol_ResizeGrip]              = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
                style->Colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                style->Colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                style->Colors[ImGuiCol_PlotLines]               = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
                style->Colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
                style->Colors[ImGuiCol_PlotHistogram]           = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
                style->Colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
                style->Colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
                style->Colors[ImGuiCol_Tab]                     = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                style->Colors[ImGuiCol_TabActive]               = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                style->Colors[ImGuiCol_TabHovered]              = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            }

            ImGui_ImplGlfw_InitForOpenGL(globals.window, true);
            ImGui_ImplOpenGL3_Init("#version 330");
        }

        /* Icon (TEMP) */
        GLFWimage icon = LoadIcon("../icon.tga", globals.window);

        /* Load Primitives */
        {
            /* Shaders */
            {
                uint32_t shaderProgram = CreateShaderProgram("../Shaders/Primitive.vert", "../Shaders/Primitive.frag");

                if (shaderProgram == uint32_t(-1)) return Result::Failure;

                globals.primShaderProgram = shaderProgram;
                
            }

            /* Triangle */
            {
                uint32_t vertBufObj, vertArrObj;
                glGenVertexArrays(1, &vertArrObj);
                glGenBuffers(1, &vertBufObj);
                glBindVertexArray(vertArrObj);
                glBindBuffer(GL_ARRAY_BUFFER, vertBufObj);
                constexpr float vertices[5*3] = 
                {
                    .0f, .5f, 1.0f, 0.0f, 0.0f,
                    -.5f, -.5f, 0.0f, 1.0f, 0.0f,
                    .5f, -.5f, 0.0f, 0.0f, 1.0f
                };
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
                glEnableVertexAttribArray(1);

                globals.triData.vertexArrayObject = vertArrObj;
                globals.triData.vertexBufferObject = vertBufObj;
            }

            /* Rect */
            {
                constexpr float vertices[] =
                {
                    0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
                    0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
                    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
                    -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
                };
                constexpr unsigned int indices[] =
                {
                    0, 1, 3,
                    1, 2, 3
                };

                uint32_t vertBufObj, vertArrObj, idxBufObj;
                glGenVertexArrays(1, &vertArrObj);
                glGenBuffers(1, &vertBufObj);
                glGenBuffers(1, &idxBufObj);
                glBindVertexArray(vertArrObj);
                glBindBuffer(GL_ARRAY_BUFFER, vertBufObj);
                glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufObj);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof (float)));
                glEnableVertexAttribArray(1);

                globals.rectData.vertexArrayObject = vertArrObj;
                globals.rectData.vertexBufferObject = vertBufObj;
                globals.rectData.indexBufferObject = idxBufObj;
            }

            /* Circle */
            {
                constexpr float vertices[] =
                {
                    -1.0f, -1.0f,
                    1.0f, -1.0f,
                    1.0f, 1.0f,
                    -1.0f, 1.0f
                };

                uint32_t vertBufObj, vertArrObj;
                glGenVertexArrays(1, &vertArrObj);
                glGenBuffers(1, &vertBufObj);
                glBindVertexArray(vertArrObj);
                glBindBuffer(GL_ARRAY_BUFFER, vertBufObj);
                glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
                glEnableVertexAttribArray(0);

                globals.circleData.vertexArrayObject = vertArrObj;
                globals.circleData.vertexBufferObject = vertBufObj;

                uint32_t shaderProgram = CreateShaderProgram("../Shaders/Circle.vert", "../Shaders/Circle.frag");

                globals.circleData.shaderProgram = shaderProgram;

                glProgramUniform1f(shaderProgram, 0, .3f);
                glProgramUniform3f(shaderProgram, 1, 1.f, .7f, .0f);
                glProgramUniform2f(shaderProgram, 2, 0.0f, 0.0f);
            }

            /* Textured Quad */
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                constexpr float vertices[] = {
                    0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                    0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
                };

                uint32_t vertBufObj, vertArrObj;
                glGenVertexArrays(1, &vertArrObj);
                glGenBuffers(1, &vertBufObj);
                glBindVertexArray(vertArrObj);
                glBindBuffer(GL_ARRAY_BUFFER, vertBufObj);
                glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof (float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof (float)));
                glEnableVertexAttribArray(2);

                uint32_t shaderProgram = CreateShaderProgram("../Shaders/TexturedQuad.vert", "../Shaders/TexturedQuad.frag");

                if (shaderProgram == uint32_t(-1)) return Result::Failure;

                globals.texturedQuadData.vertexBufferObject = vertBufObj;
                globals.texturedQuadData.vertexArrayObject = vertArrObj;
                globals.texturedQuadData.shaderProgram = shaderProgram;
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        /* Init Game */
        if (anInitCallback)
        {
            anInitCallback();
        }

        /* Game Loop */
        int timeStart, timeEnd;
        float timeElapsed;
        while (!glfwWindowShouldClose(globals.window))
        {
            timeStart = clock();

            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glClearColor(globals.clearColor.x, globals.clearColor.y, globals.clearColor.z, globals.clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);

            if (anUpdateCallback) anUpdateCallback(timeElapsed);
 
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(globals.window);

            timeEnd = clock();
            timeElapsed = float(timeEnd - timeStart) / (float)CLOCKS_PER_SEC;
        }

        /* Clean-up */
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glDeleteVertexArrays(1, &globals.triData.vertexArrayObject);
            glDeleteBuffers(1, &globals.triData.vertexBufferObject);
            glDeleteVertexArrays(1, &globals.rectData.vertexArrayObject);
            glDeleteBuffers(1, &globals.rectData.vertexBufferObject);
            glDeleteBuffers(1, &globals.rectData.indexBufferObject);
            glDeleteProgram(globals.primShaderProgram);
            glDeleteVertexArrays(1, &globals.circleData.vertexArrayObject);
            glDeleteBuffers(1, &globals.circleData.vertexBufferObject);
            glDeleteProgram(globals.circleData.shaderProgram);

            glfwTerminate();

            free(icon.pixels);
        }

        return Result::Success;
    }

    void DrawTriangle(v2 aPoint1, v2 aPoint2, v2 aPoint3, Color aColor)
    {
        const v2f n1 = PixelToNormalized(aPoint1), n2 = PixelToNormalized(aPoint2), n3 = PixelToNormalized(aPoint3);

        const v3f color = { float(aColor.r) / 255.0f, float(aColor.g) / 255.0f, float(aColor.b) / 255.0f };

        glUseProgram(globals.primShaderProgram);
        glBindVertexArray(globals.triData.vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, globals.triData.vertexBufferObject);
        const float vertices[5*3] =
        {
            n1.x, n1.y, color.x, color.y, color.z,
            n2.x, n2.y, color.x, color.y, color.z,
            n3.x, n3.y, color.x, color.y, color.z,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void DrawRect(v2 aPosition, v2 aSize, Color aColor)
    {
        const v2 halfSize = { aSize.x >> 1, aSize.y >> 1 };
        const v2f tl = PixelToNormalized({ aPosition.x - halfSize.x, aPosition.y - halfSize.y });
        const v2f tr = PixelToNormalized({ aPosition.x + halfSize.x, aPosition.y - halfSize.y });
        const v2f br = PixelToNormalized({ aPosition.x + halfSize.x, aPosition.y + halfSize.y });
        const v2f bl = PixelToNormalized({ aPosition.x - halfSize.x, aPosition.y + halfSize.y });

        const v3f color = { float(aColor.r) / 255.0f, float(aColor.g) / 255.0f, float(aColor.b) / 255.0f };

        glUseProgram(globals.primShaderProgram);
        glBindVertexArray(globals.rectData.vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, globals.rectData.vertexBufferObject);
        const float vertices[] =
        {
            tr.x, tr.y, color.x, color.y, color.z,
            br.x, br.y, color.x, color.y, color.z,
            bl.x, bl.y, color.x, color.y, color.z,
            tl.x, tl.y, color.x, color.y, color.z,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void DrawCircle(v2 aPosition, int aRadius, Color aColor)
    {
        const float radius = float(aRadius) / globals.winWidth;
        const v2f pos = PixelToNormalized(aPosition);
        const v3f color = { float(aColor.r) / 255.0f, float(aColor.g) / 255.0f, float(aColor.b) / 255.0f };

        glProgramUniform1f(globals.circleData.shaderProgram, 0, radius);
        glProgramUniform3f(globals.circleData.shaderProgram, 1, color.x, color.y, color.z);
        glProgramUniform2f(globals.circleData.shaderProgram, 2, pos.x, pos.y);
        glProgramUniform1f(globals.circleData.shaderProgram, 3, float(globals.winWidth) / float(globals.winHeight));

        glUseProgram(globals.circleData.shaderProgram);
        glBindVertexArray(globals.circleData.vertexArrayObject);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    void DrawTexturedQuad(Texture aTexture, v2 aPosition, v2 aSize, Color aColor, v4f aUV)
    {
        const v2 halfSize = { aSize.x >> 1, aSize.y >> 1 };
        const v2f tl = PixelToNormalized({ aPosition.x - halfSize.x, aPosition.y - halfSize.y });
        const v2f tr = PixelToNormalized({ aPosition.x + halfSize.x, aPosition.y - halfSize.y });
        const v2f br = PixelToNormalized({ aPosition.x + halfSize.x, aPosition.y + halfSize.y });
        const v2f bl = PixelToNormalized({ aPosition.x - halfSize.x, aPosition.y + halfSize.y });

        const v3f color = { float(aColor.r) / 255.0f, float(aColor.g) / 255.0f, float(aColor.b) / 255.0f };

        glUseProgram(globals.texturedQuadData.shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, aTexture);
        glBindVertexArray(globals.texturedQuadData.vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, globals.texturedQuadData.vertexBufferObject);
        const float vertices[] =
        {
            tr.x, tr.y, color.x, color.y, color.z, 1.0f, aUV.z, aUV.y,
            br.x, br.y, color.x, color.y, color.z, 1.0f, aUV.z, aUV.w,
            bl.x, bl.y, color.x, color.y, color.z, 1.0f, aUV.x, aUV.w,
            tl.x, tl.y, color.x, color.y, color.z, 1.0f, aUV.x, aUV.y
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    void OnWindowResize(void (*aCallback)(uint16_t, uint16_t))
    {
        globals.windowResizeCallback = aCallback;
    }

    void OnDragDrop(void (*aCallback)(const char**, size_t))
    {
        globals.dragDropCallback = aCallback;
    }

    void SetWindowTitle(const char* const aTitle)
    {
        glfwSetWindowTitle(globals.window, aTitle);
    }

    void OnInput(void (*aCallback)(int, int))
    {
        globals.keyCallback = aCallback;
    }

    void SetClearColor(Color aColor)
    {
        globals.clearColor = Normalize(aColor);
    }

    v2f GetMousePosition()
    {
        double a, b;
        glfwGetCursorPos(globals.window, &a, &b);
        return { float(a), float(b) };
    }

    Texture CreateTexture(const char* const aPath, v2* aSizeTarget)
    {
        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, channelCount;
        uint8_t* img = stbi_load(aPath, &width, &height, &channelCount, 0);

        if (!img)
        {
            glDeleteTextures(1, &texture);
            return Texture(-1);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (channelCount == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(img);

        if (aSizeTarget)
        {
            *aSizeTarget = { width, height };
        }

        return texture;
    }

    void ReturnTexture(Texture aTexture)
    {
        glDeleteTextures(1, &aTexture);
    }

    uint32_t CreateShaderProgram(const char* const aVertShader, const char* const aFragShader)
    {
        int success;
        constexpr size_t errorLogSize = 1024;
        char errorLog[errorLogSize] = "";

        FILE* vertFile = 0;
        FILE* fragFile = 0;
        vertFile = fopen(aVertShader, "rb");
        fragFile = fopen(aFragShader, "rb");
        if (!vertFile) return uint32_t(-1);
        if (!fragFile) return uint32_t(-1);
        fseek(vertFile, 0, SEEK_END);
        fseek(fragFile, 0, SEEK_END);
        const size_t vertFileSize = ftell(vertFile);
        const size_t fragFileSize = ftell(fragFile);
        char* buffer = (char*)malloc(vertFileSize + 2 + fragFileSize);
        char* fragBuffer = buffer + vertFileSize + 1;
        buffer[vertFileSize] = '\0';
        fragBuffer[fragFileSize] = '\0';
        rewind(vertFile);
        rewind(fragFile);
        fread(buffer, vertFileSize, 1, vertFile);
        fread(fragBuffer, fragFileSize, 1, fragFile);
        fclose(vertFile);
        fclose(fragFile);

        uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &buffer, 0);
        glCompileShader(vertShader);

        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertShader, errorLogSize, 0, errorLog);
            printf("Vertex Shader Error: %s\n", errorLog);
            return uint32_t(-1);
        }

        uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragBuffer, 0);
        glCompileShader(fragShader);

        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragShader, errorLogSize, 0, errorLog);
            printf("Fragment Shader Error: %s\n", errorLog);
            return uint32_t(-1);
        }

        uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        free(buffer);

        return shaderProgram;
    }

    void SetCameraPosition(v2f aPosition)
    {
        globals.cameraPosition = aPosition;
    }
    v2f GetCameraPosition()
    {
        return globals.cameraPosition;
    }
}