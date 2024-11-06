#include "file1.h"
#include "file2.h"


#include <tchar.h>
#include <windows.h>

int main() {

    sayGoodbye("World");
    sayHello("World");

    
    HINSTANCE lib;
    typedef void (__cdecl *MYPROC)(LPCSTR);
    MYPROC thatFunction;
    lib = LoadLibrary(".//dll//DLL_project.dll");
    if (lib != NULL){
        thatFunction = (MYPROC) GetProcAddress(lib, "SomeFunction");
        if (thatFunction != NULL){
            thatFunction("DLL rules!");
        }
        else{
            MessageBox(0, _T("Problems with getting function"), _T("Error"), MB_OK);
        }
        FreeLibrary(lib);
    }
    else {
        MessageBox(0, "Problems with DLL file", "Error", MB_OK);
    }

    return 0;
}