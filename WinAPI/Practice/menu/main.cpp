#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <iostream>

#include "menu.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

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
           LoadMenu(NULL, MAKEINTRESOURCE(IDR_MYMENU)),                /* No menu */
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
    int length;
    LPTSTR buffer;
    switch (
            message)                  /* handle the messages */
    {
        case WM_CREATE:
            CreateWindow(
                         "BUTTON",
                         "Press me",
                         WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                         10, 10, 100, 100,
                         hwnd,
                         (HMENU) BUTTON_1,
                         (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE),
                         NULL
                         );
                CreateWindow(
                         "BUTTON",
                         "Press ni",
                         WS_CHILD | BS_DEFPUSHBUTTON,
                         120, 10, 100, 100,
                         hwnd,
                         (HMENU) BUTTON_2,
                         (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE),
                         NULL
                         );
                // Change title
                CreateWindow(
                          "BUTTON",
                          "Change title",
                          WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                          10, 120, 100, 100,
                          hwnd,
                          (HMENU) BUTTON_3,
                          (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE),
                          NULL
                          );

                CreateWindow(
                          "EDIT",
                          "Title",
                          WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                          120, 120, 100, 100,
                          hwnd,
                          (HMENU) BUTTON_INPUT,
                          (HINSTANCE) GetWindowLong(hwnd, GWLP_HINSTANCE),
                          NULL
                          );
        break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case BUTTON_1:
                    //DestroyWindow(hwnd);
                    //SendMessage(hwnd, WM_CLOSE, 0, 0);
                    //ShowWindow(GetDlgItem(hwnd, BUTTON_1), SW_HIDE);
                    //length = GetWindowTextLength(GetDlgItem(hwnd, EDstdIT_FIELD));
                    //length = SendMessage(GetDlgItem(hwnd, EDIT_FIELD),WM_GETTEXTLENGTH, 0, 0);
                    //buffer = new TCHAR[length+1];
                    //GetWindowText(GetDlgItem(hwnd, EDIT_FIELD), buffer, length+1);
                    //SetWindowText(hwnd, buffer);
                    //DialogBox( NULL, MAKEINTRESOURCE(MYDIALOG), hwnd, (DLGPROC)DialogProc );
                    ShowWindow(GetDlgItem(hwnd, BUTTON_2), SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, BUTTON_1), SW_HIDE);
                    break;
                case BUTTON_2:
                    ShowWindow(GetDlgItem(hwnd, BUTTON_1), SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, BUTTON_2), SW_HIDE);
                    break;
                case BUTTON_3:
                    {
                        HWND input = GetDlgItem(hwnd, BUTTON_INPUT);
                        int inputLength = GetWindowTextLength(input);
                        LPTSTR inputText = new TCHAR[inputLength + 1];
                        GetWindowText(input, inputText, inputLength + 1);
                        SetWindowText(hwnd, inputText);
                    }
                    break;
                case BUTTON_INPUT:
                    //std::cout<<"aosidfjas;odifj";
                    break;
                case ID_FILE_EXIT:
                    DestroyWindow(hwnd);
                break;
                case ID_STUFF_GO:
                    MessageBox(hwnd, "YAY", "Title", MB_OK);
                break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPTSTR edittxt;
    switch(uMsg)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case DIALOG_BUTTON:
                    HWND edit = GetDlgItem(hDlg, TEKSTAS);
                    int length = GetWindowTextLength(edit);
                    edittxt = new TCHAR[length+1];
                    GetWindowText(edit, edittxt, length+1);
                    SetWindowText(GetParent(hDlg), edittxt);
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
	    return TRUE;
        case WM_CLOSE:
            EndDialog(hDlg, 0);
            return TRUE;
        case WM_DESTROY:
            EndDialog(hDlg, 0);
            return TRUE;
    }
    return FALSE;
}
