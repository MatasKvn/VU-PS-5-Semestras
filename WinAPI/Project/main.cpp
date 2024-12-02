
#include <tchar.h>
#include <windows.h>

#include <d2d1.h>
#include <wincodec.h>

#include <iostream>
#include <assert.h>

template <class T>
void SafeRelease(T** ppT)
{
    if (ppT)
    {
        (*ppT)->Release();
        ppT = NULL;
    }
}

TCHAR szClassName[] = _T("ClassName xd");

ID2D1Factory* pD2DFactory = nullptr;
IWICImagingFactory* pWICFactory = nullptr;
ID2D1HwndRenderTarget* pRenderTarget = nullptr;

void InitDirect2D(HWND hwnd) {
    // Create Direct2D factory
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

    // Create WIC factory
    HRESULT res = CoInitialize(NULL);
    assert(SUCCEEDED(res));
    res = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
    assert(SUCCEEDED(res));

    // Create a render target
    RECT rc;
    GetClientRect(hwnd, &rc);
    pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right, rc.bottom)),
        &pRenderTarget
    );

}

HRESULT hr2 = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));

ID2D1Bitmap* LoadBitmapFromFile(LPCWSTR uri, ID2D1HwndRenderTarget* pRenderTarget) {
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;
    ID2D1Bitmap* pD2DBitmap = nullptr;

    // Decode the image file
    HRESULT hr = pWICFactory->CreateDecoderFromFilename(
        uri, 
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr)) {
        // Get the first frame of the image
        hr = pDecoder->GetFrame(0, &pFrame);
    }
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr)) {
        // Create a format converter
        hr = pWICFactory->CreateFormatConverter(&pConverter);
    }
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr)) {
        // Convert the frame to 32bppPBGRA, required for Direct2D
        hr = pConverter->Initialize(
            pFrame, 
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeCustom
        );
    }
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr)) {
        // Create a Direct2D bitmap from the WIC bitmap
        hr = pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter, 
            NULL,
            &pD2DBitmap
        );
    }
    assert(SUCCEEDED(hr));

    // Clean up WIC objects
    SafeRelease(&pDecoder);
    SafeRelease(&pFrame);
    SafeRelease(&pConverter);

    return pD2DBitmap;
}


void OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);

    // Ensure render target is valid
    if (pRenderTarget == nullptr) {
        InitDirect2D(hwnd);
    }

    // Load the bitmap
    ID2D1Bitmap* pBitmap = LoadBitmapFromFile(L"cool.bmp", pRenderTarget);

    if (pRenderTarget) {
        pRenderTarget->BeginDraw();
        // pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));


        if (pBitmap) {
            // Draw the image on the render target
            pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            // pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(20, D2D1::Point2F(55, 55)));
            std::cout << pBitmap->GetSize().width << std::endl;
            pRenderTarget->DrawBitmap(
                pBitmap,
                D2D1::RectF(10, 10, 500+10, 500+10),
                1.0f,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                D2D1::RectF(0, 0, 20, 20)
            );
            perror("ERROR: ");
        }

        HRESULT hr = pRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET) {
            SafeRelease(&pRenderTarget);
        }
    }

    // Clean up
    SafeRelease(&pBitmap);
    EndPaint(hwnd, &ps);
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

                // Direct2d rectangle
                RECT rect;
                GetClientRect(hwnd, &rect);

                ID2D1SolidColorBrush* brush = NULL;
                pRenderTarget->CreateSolidColorBrush(
                    D2D1::ColorF(D2D1::ColorF::Red),
                    &brush
                );
                pRenderTarget->BeginDraw();

                pRenderTarget->DrawRectangle(
                        D2D1::RectF(
                            rect.left + 10.0f,
                            rect.top + 10.0f,
                            rect.right - 10.0f,
                            rect.bottom - 10.0f
                        ),
                        brush
                    );

                HRESULT hrr = pRenderTarget->EndDraw();  
                SafeRelease(&brush);

                // Direct2 image

                OnPaint(hwnd);

                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                EndPaint(hwnd, &ps);
            }
            break;
        case WM_SIZE: 
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            break;
        }
        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    // HWND hwnd;
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
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szClassName,
        _T("Window"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, hInstance, NULL
    );
    InitDirect2D(hwnd);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if(hwnd == NULL)
    {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg); 
        DispatchMessage(&Msg);
    }
    SafeRelease(&pRenderTarget);
    SafeRelease(&pD2DFactory);
    

    return 0;
}