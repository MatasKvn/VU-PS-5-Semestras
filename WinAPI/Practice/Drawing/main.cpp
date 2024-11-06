#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "drawing.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void draw_image(HWND);
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

BOOL LoadBMP(HWND hwnd, LPCTSTR fileName)
{
    HDC hdc = GetDC(hwnd);

    HANDLE hbmp = LoadImage(NULL, fileName, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
    if (hbmp == NULL) {
        MessageBox(hwnd, "No image found", "Error", MB_OK);
        return FALSE;
    }

    HDC dcmem = CreateCompatibleDC(NULL);
    SelectObject(dcmem, hbmp);

    BITMAP bmp;
    GetObject(hbmp, sizeof(bmp), &bmp);

    BitBlt(hdc, 200, 200, 100, 100, dcmem, 0, 0, SRCCOPY);
    return TRUE;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(hThisInstance, MAKEINTRESOURCE(ID_MENU_MAIN)),                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HBRUSH old_brush, blueBrush;
    HPEN red_pen, old_pen;
    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case IDM_FILE_OPEN:
                    draw_image(hwnd);
                break;
            }
        break;
        case WM_PAINT:
         //1 TASK. Fill ellipse with some color
         //2 TASK. Change line style to dash-dot and color to red
            HDC hdc;
            PAINTSTRUCT ps;
            blueBrush=CreateSolidBrush(RGB(0,0,255));
            red_pen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
            hdc = BeginPaint(hwnd, &ps);
            old_brush = (HBRUSH)SelectObject(hdc, blueBrush);
            old_pen  = (HPEN)SelectObject(hdc, red_pen);
            Ellipse(hdc, 30, 30, 200, 200);
            SelectObject(hdc, old_brush);
            SelectObject(hdc, old_pen);
            DeleteObject(blueBrush);
            DeleteObject(red_pen);

            LoadBMP(hwnd, "image.bmp");
            EndPaint(hwnd, &ps);
        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
void draw_image(HWND hwnd){
    HDC hdc;
    hdc = GetDC(hwnd);
    //Need to check
    HANDLE bmp = LoadImage(NULL,"fun.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (bmp==NULL){
        MessageBox(hwnd, "Failed to load image", "Error", MB_OK|MB_ICONERROR);
        return ;
    }
    HDC dcmem = CreateCompatibleDC(NULL);
    if (SelectObject(dcmem, bmp)==NULL){
        MessageBox(hwnd, "Failed to upload", "Error", MB_OK|MB_ICONERROR);
        DeleteDC(dcmem);
        return ;
    }
    BITMAP bm;
    GetObject(bmp, sizeof(bm), &bm);
    if (BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, dcmem, 0, 0, SRCCOPY) == 0){
        MessageBox(hwnd, "Failed to blit", "Error", MB_OK|MB_ICONERROR);
        DeleteDC(dcmem);
        return ;
    }
    DeleteDC(dcmem);
}
