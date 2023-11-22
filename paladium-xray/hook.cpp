#include <Windows.h>
#include <gl/GL.h>
#include "hook.hpp"
#include "xray.hpp"

#pragma comment(lib, "minhook/minhook.lib")
#include "minhook/MinHook.h"

typedef BOOL(__stdcall* def_wglSwapBuffers) (HDC hDc);
def_wglSwapBuffers orig_wglSwapBuffers = nullptr;

RECT last_rect;

template <typename T>
inline MH_STATUS MH_CreateHookApiEx(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHookApi(pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

bool should_reinitialize(RECT rect)
{
	return rect.left != last_rect.left || rect.right != last_rect.right || rect.top != last_rect.top || rect.bottom != last_rect.bottom;
}

bool hook_wglSwapBuffers(HDC hDc)
{
	static HGLRC current_context = wglGetCurrentContext();
	static HGLRC context = wglCreateContext(hDc);
	static bool initialized = false;

	HWND hwnd = WindowFromDC(hDc);
	RECT rect;

	GetClientRect(hwnd, &rect);

	if (!initialized)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glViewport(0, 0, viewport[2], viewport[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);

		initialized = true;
		last_rect = rect;

		return orig_wglSwapBuffers(hDc);
	}

	if (should_reinitialize(rect)) 
	{
		wglDeleteContext(context);
		context = wglCreateContext(hDc);

		initialized = false;
		return hook_wglSwapBuffers(hDc);
	}

	wglMakeCurrent(hDc, context);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
	xray::render();
	glPopMatrix();

	wglMakeCurrent(hDc, current_context);
	return orig_wglSwapBuffers(hDc);
}

void hook::initialize_hooks()
{
	MH_Initialize();

	// get wglSwapBuffers addr
	HMODULE opengl_handle = GetModuleHandleA("opengl32.dll");

	if (!opengl_handle)
	{
		MessageBoxA(NULL, "No OpenGL module found", "Message", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (MH_CreateHookApiEx(L"opengl32.dll", "wglSwapBuffers", &hook_wglSwapBuffers, &orig_wglSwapBuffers) != MH_OK)
	{
		MessageBoxA(NULL, "Could not create wglSwapBuffers hook", "Message", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		MessageBoxA(NULL, "Could not enable hooks", "Message", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void hook::uninitialize_hooks()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}