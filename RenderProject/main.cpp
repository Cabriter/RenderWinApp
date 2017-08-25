#include "Windows.h"
#include "RenderWinApp.hpp"

int __stdcall
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	RenderWinApp app(hInstance);
	app.main(800, 600);

	return 0;
}