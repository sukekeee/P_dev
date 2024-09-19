#include <windows.h>
#include <stdio.h>

// グローバル変数
HWND hEdit;    // テキストボックスのハンドル
HWND hButton;  // OKボタンのハンドル
HFONT hFont;   // フォントのハンドル
char buffer[256];  // 入力された文字列を格納するバッファ

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        // テキストボックスを作成
        hEdit = CreateWindowEx(
            0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            50, 50, 200, 20, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // OKボタンを作成
        hButton = CreateWindowEx(
            0, "BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 80, 100, 30, hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // フォントを作成 (高さ16ポイント、幅は自動調整、Arialフォント)
        hFont = CreateFont(
            -16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, "Arial");

        // テキストボックスとボタンにフォントを適用
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 2) {
            // OKボタンが押されたときの処理
            GetWindowText(hEdit, buffer, sizeof(buffer));  // テキストボックスから文字列を取得
            printf("入力された文字列: %s\n", buffer);    // 取得した文字列を表示

            // ウィンドウを閉じる
            DestroyWindow(hwnd);
        } else if (HIWORD(wParam) == EN_UPDATE && LOWORD(wParam) == 1) {
            // テキストボックスでEnterキーが押された場合
            if (GetKeyState(VK_RETURN) & 0x8000) { // Enterキーの状態を確認
                GetWindowText(hEdit, buffer, sizeof(buffer));  // テキストボックスから文字列を取得
                printf("入力された文字列: %s\n", buffer);    // 取得した文字列を表示

                // ウィンドウを閉じる
                DestroyWindow(hwnd);
            }
        }
        break;

    case WM_DESTROY:
        // フォントを削除
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// WinMain関数 - プログラムのエントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // コンソールウィンドウを表示
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // printfを使ってコンソールに出力するための準備

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "WindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 背景色を白に設定

    if (!RegisterClass(&wc)) {
        return -1;
    }

    HWND hwnd = CreateWindowEx(
        0, "WindowClass", "C言語ウィンドウ", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return -1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // コンソールウィンドウを閉じる
    FreeConsole();
    return (int)msg.wParam;
}
