// WINDOWS
#include <windows.h>
#include <tchar.h>

// OpenGL
#include <GL/gl.h>
#include <GL/glu.h>

// std lib
#include <iostream>
#include <vector>


TCHAR szClassName[] = _T("myWindowClass");

// Triangles
struct Triangle {
        GLfloat vertices[3][3];
        GLfloat colors[3][3];
};

void drawTriangle(const Triangle& triangle) {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; ++i) {
        glColor3f(triangle.colors[i][0], triangle.colors[i][1], triangle.colors[i][2]);
        glVertex3f(triangle.vertices[i][0], triangle.vertices[i][1], triangle.vertices[i][2]);
    }
    glEnd();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
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
            glViewport(0, 0, width, height);
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

    // Initialize OpenGL
    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;
    // pfd.iLayerType = PFD_MAIN_PLANE;
    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);
    HGLRC hrc = wglCreateContext(hdc);

    wglMakeCurrent(hdc, hrc);
    BOOL bResult = SetWindowPos(hwnd, HWND_TOP, 0, 0, 400, 400, SWP_SHOWWINDOW);

    

    std::vector<Triangle> triangles;
    Triangle triangle = {
        { {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f} },  // Vertices
        { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }     // Colors
    };
    Triangle triangle2 = {
        { {0.0f, -0.5f, 0.5f}, {0.0f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.0f} },  // Vertices
        { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }     // Colors
    };

    triangles.push_back(triangle);
    // triangles.push_back(triangle2);

    float angle = 0.0f;

    

    // Main loop
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Clear the screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Reset transformations
            glLoadIdentity();

            // Set rotation
            glRotatef(angle, 0, 1, 0);

            // Draw something (e.g., a triangle)
            for (auto& triangle : triangles) {
                drawTriangle(triangle);
            }

            
            angle += 5;

            // Swap the buffers
            SwapBuffers(hdc);

            Sleep(1);
        }
    }

    ReleaseDC(hwnd, hdc);
    wglDeleteContext(hrc);
    DestroyWindow(hwnd);
    return Msg.wParam;
}
