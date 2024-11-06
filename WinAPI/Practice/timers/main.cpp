#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "timers.h"
#include <stdio.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

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
           NULL,                /* No menu */
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
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            CreateWindow(
                "BUTTON",  // Predefined class; Unicode assumed
                "OK",      // Button text
                WS_TABSTOP | WS_VISIBLE | WS_CHILD |  BS_DEFPUSHBUTTON/*BS_ICON*/,  // Styles
                10,         // x position
                10,         // y position
                100,        // Button width
                100,        // Button height
                hwnd,     // Parent window
                (HMENU) BUTTON_ID,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),
                NULL);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
            case BUTTON_ID:
                SetTimer(hwnd, TIMER_2, 2000, (TIMERPROC) TimerProc);
                SetTimer(hwnd, TIMER_1, 3000, NULL);

                break;
            }
            break;
        case WM_TIMER:
            switch(LOWORD(wParam)){
            case TIMER_1:
                KillTimer(hwnd, TIMER_1);
                MessageBox(hwnd, "Timer run out of time", "Timer", MB_OK);
                break;
            }
            break;
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
VOID CALLBACK TimerProc(
    HWND hwnd,        // handle to window for timer messages
    UINT message,     // WM_TIMER message
    UINT idTimer,     // timer identifier
    DWORD dwTime)     // current system time
{
    SYSTEMTIME st;
     switch(message){
        case WM_TIMER:
            if (idTimer == TIMER_2){

                KillTimer(hwnd, TIMER_2);
                MessageBox(hwnd, "TimerProcedure with TIMER_2", "Timer", MB_OK);
                //TASK. Show current time in MessageBox
                GetLocalTime(&st);
                char s_time[50];
                sprintf(s_time, "Local time: %02d:%02d\n", st.wHour, st.wMinute);
                MessageBox(hwnd, s_time, "Timer", MB_OK);
            }
            break;
    }
}
