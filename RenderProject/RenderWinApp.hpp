#pragma once
#include "Windows.h"
class RenderWinApp
{
protected:
	HWND _hWnd;
	HINSTANCE _hInst;
public:
	RenderWinApp(HINSTANCE hInstance):_hInst(hInstance) {
		//生成一个窗口类
		WNDCLASSW wndClass;
		memset(&wndClass, 0, sizeof(wndClass));
		wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wndClass.lpszClassName = (LPCWSTR)("RenderWinApp");
		wndClass.lpfnWndProc = wndProc;
		wndClass.hIcon = 0;
		wndClass.lpszMenuName = 0;

		RegisterClass(&wndClass);
	}
	~RenderWinApp() {
		UnregisterClass((LPCWSTR)("RenderWinApp"), _hInst);
	}

	static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		
		RenderWinApp* pThis = (RenderWinApp*)GetWindowLong(hWnd, GWL_USERDATA);
		if (pThis) {
			return pThis->onEvent(hWnd, msg, wParam, lParam);
		}
		if (WM_CREATE == msg) {
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pCreate->lpCreateParams);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg)
		{
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_KEYDOWN: {
			switch (wParam)
			{
			case VK_LEFT:
				break;
			default:
				break;
			}
		}
			break;
		case WM_KEYUP:
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}
		return S_OK;
	}
	int main(int width, int hight) {
		//2创建窗口
		_hWnd = CreateWindow((LPCWSTR)("RenderWinApp"), (LPCWSTR)("RenderWinApp"), WS_OVERLAPPEDWINDOW, 0, 0, width, hight, 0, 0, _hInst, this);
		if (_hWnd == 0) return 0;
		//3更新显示窗口
		UpdateWindow(_hWnd);
		//4显示窗口
		ShowWindow(_hWnd, SW_MAXIMIZE);
		//5消息循环
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (msg.message == WM_DESTROY || msg.message == WM_CLOSE) {
				break;
			}

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				Sleep(1);
			}
		}
		return 0;
	}

};

