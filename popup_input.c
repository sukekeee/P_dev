#include <windows.h>
#include <stdio.h>

// �O���[�o���ϐ�
HWND hEdit;    // �e�L�X�g�{�b�N�X�̃n���h��
HWND hButton;  // OK�{�^���̃n���h��
HFONT hFont;   // �t�H���g�̃n���h��
char buffer[256];  // ���͂��ꂽ��������i�[����o�b�t�@

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        // �e�L�X�g�{�b�N�X���쐬
        hEdit = CreateWindowEx(
            0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            50, 50, 200, 20, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // OK�{�^�����쐬
        hButton = CreateWindowEx(
            0, "BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 80, 100, 30, hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // �t�H���g���쐬 (����16�|�C���g�A���͎��������AArial�t�H���g)
        hFont = CreateFont(
            -16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, "Arial");

        // �e�L�X�g�{�b�N�X�ƃ{�^���Ƀt�H���g��K�p
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 2) {
            // OK�{�^���������ꂽ�Ƃ��̏���
            GetWindowText(hEdit, buffer, sizeof(buffer));  // �e�L�X�g�{�b�N�X���當������擾
            printf("���͂��ꂽ������: %s\n", buffer);    // �擾�����������\��

            // �E�B���h�E�����
            DestroyWindow(hwnd);
        } else if (HIWORD(wParam) == EN_UPDATE && LOWORD(wParam) == 1) {
            // �e�L�X�g�{�b�N�X��Enter�L�[�������ꂽ�ꍇ
            if (GetKeyState(VK_RETURN) & 0x8000) { // Enter�L�[�̏�Ԃ��m�F
                GetWindowText(hEdit, buffer, sizeof(buffer));  // �e�L�X�g�{�b�N�X���當������擾
                printf("���͂��ꂽ������: %s\n", buffer);    // �擾�����������\��

                // �E�B���h�E�����
                DestroyWindow(hwnd);
            }
        }
        break;

    case WM_DESTROY:
        // �t�H���g���폜
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// WinMain�֐� - �v���O�����̃G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // �R���\�[���E�B���h�E��\��
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // printf���g���ăR���\�[���ɏo�͂��邽�߂̏���

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "WindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // �w�i�F�𔒂ɐݒ�

    if (!RegisterClass(&wc)) {
        return -1;
    }

    HWND hwnd = CreateWindowEx(
        0, "WindowClass", "C����E�B���h�E", WS_OVERLAPPEDWINDOW,
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

    // �R���\�[���E�B���h�E�����
    FreeConsole();
    return (int)msg.wParam;
}
