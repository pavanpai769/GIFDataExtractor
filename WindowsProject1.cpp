#include <windows.h>
#include <iostream>
#include <string>
#include "WindowState.cpp"
#include "GIF.cpp"
#include "Frame.cpp"
#include "LZWDecompressor.cpp"
#include "GIFDataExtractor.cpp"
#include <vector>

WindowState* windowState = new WindowState();

GIFDataExtractor extractor("C:\\Users\\pavan\\Downloads\\file2.gif");
GIF gif = extractor.extractGIF();

int gifWidth = 0;
int gifHeight = 0;

struct DecodedFrame
{
    std::vector<uint8_t> pixels;
    BITMAPINFO bmi;
    int width;
    int height;
};

std::vector<DecodedFrame> decodedFrames;

int currentFrame = 0;

bool isPlaying = true;

void LoadFrame(int frameIndex)
{
    const Frame& firstFrame = gif.getFrames()[frameIndex];

    LZWDecompressor decompressor;

    std::vector<uint8_t> indices =
        decompressor.decompress(
            firstFrame.getCompressedData(),
            firstFrame.getMinimumCodeSize()
        );

    if (firstFrame.getImageDescriptor()
        .getPackedFlag()
        .isInterlaced())
    {
        std::vector<uint8_t> reordered(indices.size());

        int width = firstFrame.getImageDescriptor().getWidth();
        int height = firstFrame.getImageDescriptor().getHeight();

        int srcRow = 0;

        for (int y = 0; y < height; y += 8)
        {
            memcpy(&reordered[y * width],
                &indices[srcRow * width],
                width);
            srcRow++;
        }

        for (int y = 4; y < height; y += 8)
        {
            memcpy(&reordered[y * width],
                &indices[srcRow * width],
                width);
            srcRow++;
        }

        for (int y = 2; y < height; y += 4)
        {
            memcpy(&reordered[y * width],
                &indices[srcRow * width],
                width);
            srcRow++;
        }

        for (int y = 1; y < height; y += 2)
        {
            memcpy(&reordered[y * width],
                &indices[srcRow * width],
                width);
            srcRow++;
        }

        indices = std::move(reordered);
    }

    const ColorTable* colorTable;

    if (firstFrame.hasLct())
        colorTable = &firstFrame.getLct().value();
    else
        colorTable = &gif.getGct().value();

    int width = firstFrame.getImageDescriptor().getWidth();
    int height = firstFrame.getImageDescriptor().getHeight();

    const auto& colors = colorTable->getColors();

    DecodedFrame decoded;

    int stride = ((width * 3 + 3) & ~3);

    decoded.pixels.resize(stride * height);
    memset(decoded.pixels.data(), 0, decoded.pixels.size());

    for (int y = 0; y < height; y++)
    {
        uint8_t* row = decoded.pixels.data() + y * stride;

        for (int x = 0; x < width; x++)
        {
            int srcIndex = y * width + x;
            uint8_t index = indices[srcIndex];

            if (firstFrame.getGce().has_value() &&
                firstFrame.getGce()->getPackedFlag().isTrasparentColorExsists() &&
                index == firstFrame.getGce()->getTransparentColor())
            {
                continue;
            }
            else
            {
                ColorTable::Color color = colors[index];

                row[x * 3 + 0] = color.b;
                row[x * 3 + 1] = color.g;
                row[x * 3 + 2] = color.r;
            }
        }
    }

    decoded.width = width;
    decoded.height = height;

    ZeroMemory(&decoded.bmi, sizeof(decoded.bmi));

    decoded.bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    decoded.bmi.bmiHeader.biWidth = width;
    decoded.bmi.bmiHeader.biHeight = -height;
    decoded.bmi.bmiHeader.biPlanes = 1;
    decoded.bmi.bmiHeader.biBitCount = 24;
    decoded.bmi.bmiHeader.biCompression = BI_RGB;

    decodedFrames.push_back(std::move(decoded));
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
    {
        isPlaying = !isPlaying;

        if (isPlaying)
        {
            SetTimer(hWnd, 1, 100, nullptr);
        }
        else
        {
            KillTimer(hWnd, 1);
        }

        return 0;
    }

    case WM_SIZE:
    {
        windowState->setWidth(LOWORD(lParam));
        windowState->setHeight(HIWORD(lParam));

        return 0;
    }

    /*
    case WM_ERASEBKGND:
        return 1;
    */

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (decodedFrames.empty())
        {
            EndPaint(hWnd, &ps);
            return 0;
        }

        const auto& frame = decodedFrames[currentFrame];

        float scale = min(
            windowState->getWidth() / (float)frame.width,
            windowState->getHeight() / (float)frame.height
        );

        int drawWidth = (int)(frame.width * scale);
        int drawHeight = (int)(frame.height * scale);

        int x = (windowState->getWidth() - drawWidth) / 2;
        int y = (windowState->getHeight() - drawHeight) / 2;

        StretchDIBits(
            hdc,
            x,
            y,
            drawWidth,
            drawHeight,
            0,
            0,
            frame.width,
            frame.height,
            frame.pixels.data(),
            &frame.bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_TIMER:
    {
        currentFrame++;

        if (currentFrame >= gif.getFrames().size())
        {
            currentFrame = 0;
        }
        InvalidateRect(hWnd, nullptr, FALSE);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"MainWindow";
    RegisterClassExW(&wcex);


    HWND hWnd = CreateWindowW(L"MainWindow", L"My Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, windowState->getWidth(), windowState->getHeight(), nullptr, nullptr, hInstance, nullptr);

    for (int i = 0; i < gif.getFrames().size(); i++)
    {
        LoadFrame(i);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetTimer(
        hWnd,
        1,
        100,
        nullptr
    );

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}