
#include <glad/glad.h>


// WINDOWS
#include <windows.h>
#include <tchar.h>

// std lib
#include <iostream>
#include <vector>

#include <math.h>

TCHAR szClassName[] = _T("myWindowClass");

void *GetAnyGLFuncAddress(const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                // TODO: Add any drawing code here...

                EndPaint(hwnd, &ps);
            }
            break;
        case WM_SIZE: 
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            if (glViewport != NULL) glViewport(0, 0, width, height);
            break;
        }
        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szClassName,
        _T("Win32 API Presentation"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL)
    {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    // Bind Context
    HDC hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pixelFormatDesc;
    pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelFormatDesc.nVersion = 1;
    pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDesc.cColorBits = 32;
    pixelFormatDesc.cAlphaBits = 8;
    pixelFormatDesc.cDepthBits = 24;
    pixelFormatDesc.cStencilBits = 8;
    pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pixelFormatDesc);
    SetPixelFormat(hdc, pixelFormat, &pixelFormatDesc);
    
    HGLRC glContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, glContext);

    // Load OpenGL Functions
    if (!gladLoadGLLoader((GLADloadproc)GetAnyGLFuncAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float angle = 0;

    // Main loop
    MSG msg;
    bool isRunning = true;
    while (isRunning)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                isRunning = false;
                continue;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        // Shaders & drawing
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };  

        UINT vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);  
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 3. then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); 

        





        // // Rendering
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // glBegin(GL_TRIANGLES); // Begin drawing triangle
        //     glColor3f(1.0f, 0.0f, 0.0f); // Red
        //     glVertex2f(0.0f, 0.5f);     // Top
        //     glColor3f(0.0f, 1.0f, 0.0f); // Green
        //     glVertex2f(-0.5f, -0.5f);   // Bottom left
        //     glColor3f(0.0f, 0.0f, 1.0f); // Blue
        //     glVertex2f(0.5f, -0.5f);    // Bottom right
        // glEnd(); // End drawing triangle

        SwapBuffers(hdc);  // Swap the double-buffer
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(glContext);
    ReleaseDC(hwnd, hdc);
    return Msg.wParam;
}

