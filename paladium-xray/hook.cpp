#include <Windows.h>
#include <gl/GL.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_win32.h"

#include "hook.hpp"
#include "xray.hpp"

#pragma comment(lib, "minhook/minhook.lib")
#include "minhook/MinHook.h"

typedef BOOL(__stdcall* def_wglSwapBuffers) (HDC hDc);
def_wglSwapBuffers orig_wglSwapBuffers = nullptr;

typedef LRESULT(CALLBACK* Orig_WindowProc) (
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Orig_WindowProc wnd_proc = nullptr;

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

void render_imgui(HDC hDc, RECT rect)
{
	if (!xray::gui_open)
	{
		return;
	}

	auto& io = ImGui::GetIO();
	io.DisplaySize.x = rect.right - rect.left;
	io.DisplaySize.y = rect.bottom - rect.top;

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplOpenGL2_NewFrame();
	ImGui::NewFrame();
	
	xray::render_gui();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

bool hook_wglSwapBuffers(HDC hDc)
{
	static HGLRC current_context = wglGetCurrentContext();
	static HGLRC context = wglCreateContext(hDc);
	static HGLRC twod_context = wglCreateContext(hDc);
	static bool initialized = false;

	HWND hwnd = WindowFromDC(hDc);
	RECT rect;

	GetClientRect(hwnd, &rect);

	if (!initialized)
	{
		wglMakeCurrent(hDc, twod_context);

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

		ImGui_ImplOpenGL2_DestroyFontsTexture();
		ImGui_ImplOpenGL2_CreateFontsTexture();

		wglMakeCurrent(hDc, current_context);
		return orig_wglSwapBuffers(hDc);
	}

	if (should_reinitialize(rect)) 
	{
		wglDeleteContext(context);
		context = wglCreateContext(hDc);

		initialized = false;
		return hook_wglSwapBuffers(hDc);
	}

	wglMakeCurrent(hDc, twod_context);
	render_imgui(hDc, rect);
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

static LRESULT WINAPI hook_WndProcHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (xray::gui_open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	if (msg == WM_KEYDOWN && wParam == VK_INSERT)
	{
		xray::gui_open = !xray::gui_open;
	}

	return CallWindowProc(wnd_proc, hWnd, msg, wParam, lParam);
}

void hook::initialize_hooks()
{
	MH_Initialize();

	wnd_proc = reinterpret_cast<Orig_WindowProc>(SetWindowLongPtr(FindWindowA("LWJGL", NULL), GWLP_WNDPROC, (LONG_PTR)hook_WndProcHandle));

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
	SetWindowLongPtr(FindWindowA("LWJGL", NULL), GWLP_WNDPROC, (LONG_PTR)wnd_proc);
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}