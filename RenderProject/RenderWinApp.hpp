#pragma once
#include "Windows.h"

#include "CELLMath.hpp"
#include "RenderShader.hpp"

#include "egl\egl.h"
#include "gles2\gl2.h"

class RenderWinApp
{
protected:
	HWND _hWnd;
	HINSTANCE _hInst;

	int _width;
	int _height;

	EGLConfig _config;
	EGLSurface _surface;
	EGLContext _context;
	EGLDisplay _display;

	PROGRAM_P2_C4 _shader;

public:
	RenderWinApp(HINSTANCE hInstance) :_hInst(hInstance) {
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

	bool initOpenGLES20() {
		const EGLint attribs[]{
			EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
			EGL_DEPTH_SIZE,8,
			EGL_GREEN_SIZE,8,
			EGL_RED_SIZE,8,
			EGL_DEPTH_SIZE,24,
			EGL_NONE
		};

		EGLint format(0);
		EGLint numConfigs(0);
		EGLint major;
		EGLint minor;

		_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		eglInitialize(_display, &major, &minor);
		eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

		eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);

		_surface = eglCreateWindowSurface(_display, _config, _hWnd, NULL);

		EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE,EGL_NONE };

		_context = eglCreateContext(_display, _config, 0, attr);

		if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE) {
			return false;
		}

		eglQuerySurface(_display, _surface, EGL_WIDTH, &_width);
		eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

		return true;
	}

	void destroyOpenGLES20() {
		if (_display != EGL_NO_DISPLAY) {
			eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (_context != EGL_NO_CONTEXT) {
				eglDestroyContext(_display, _context);
			}
			if (_surface != EGL_NO_SURFACE) {
				eglDestroySurface(_display, _surface);
			}
			eglTerminate(_display);
		}
		_display = EGL_NO_DISPLAY;
		_context = EGL_NO_CONTEXT;
		_surface = EGL_NO_SURFACE;
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
		switch (msg)
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

	void render() {

		//glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _width, _height);

		CELL::matrix4 screenProj = CELL::ortho<float>(0, float(_width), float(_height), 0, -100.0f, 100);
		_shader.begin(); 
		{
			float x = 100;
			float y = 100;
			float w = 100;
			float h = 100;
			CELL::float2 pos[] = {
				CELL::float2(x,y),
				CELL::float2(x + w,y),
				CELL::float2(x,y + h),
				CELL::float2(x + w,y + h)
			};
			glUniformMatrix4fv(_shader._MVP, 1, false, screenProj.data());
			glUniform4f(_shader._color, 1, 0, 0, 1);
			glVertexAttribPointer(_shader._position, 2, GL_FLOAT, false, sizeof(CELL::float2), pos);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		_shader.end();
	}

	int main(int width, int height) {
		_width = width;
		_height = height;

		_hWnd = CreateWindow((LPCWSTR)("RenderWinApp"), (LPCWSTR)("RenderWinApp"), WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, _hInst, this);
		if (_hWnd == 0) {
			return -1;
		}

		ShowWindow(_hWnd, SW_SHOW);

		if (!initOpenGLES20()) {
			return false;
		}
		_shader.initialize();
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
				render();
				eglSwapBuffers(_display, _surface);
			}
		}
		destroyOpenGLES20();
		return 0;
	}

};

