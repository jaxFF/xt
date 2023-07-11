#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#include "opengl/opengl.h"
#include "opengl/opengl_imports.h"
#include "opengl/opengl.cpp"
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM "opengl/opengl.h"
#define IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
#define IMGUI_DISABLE_WIN32_FUNCTIONS
#include <imgui.h>
#include <imgui.cpp>
#include <imgui_demo.cpp>
#include <imgui_draw.cpp>
#include <imgui_widgets.cpp>
#include <imgui_impl_win32.h>
#include <imgui_impl_win32.cpp>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3.cpp>

#include "array.h"
#include "getline.c"
#include "editor.h"
#include "editor.cpp"

static bool GlobalRunning = false;

LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
    ImGui_ImplWin32_WndProcHandler(Window, Message, wParam, lParam);

	LRESULT Result = 0;

	switch (Message) {
    case WM_ACTIVATEAPP: {

    } break;

    case WM_DESTROY: {
        // todo(jax): Handle this as an error -- recreate window?? how about swapping renderers mid game?
        GlobalRunning = false;
    } break;

    case WM_CLOSE: {
        // todo(jax): Handle this with a message to user
        GlobalRunning = false;
    } break;

#if 0
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    // todo(jax): RAW INPUT
    case WM_INPUT: {
        Assert(!"Keyboard input came in through a non-dispatch message!");
    } break;
#endif

    case WM_PAINT: {
        
    } break;

    default: {
        Result = DefWindowProcA(Window, Message, wParam, lParam);
    } break;
	}

	return Result;
}

int main(int ArgCount, char** Args) {
    HINSTANCE Instance = GetModuleHandleA(0);
    HWND Window = {};

	WNDCLASSA WindowClass = {};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = "xt Demo App";

    if (RegisterClassA(&WindowClass)) {
        Window = CreateWindowExA(0, //WS_EX_TOPMOST | WS_EX_LAYERED, 
			"xt Demo App",
			"xt Demo App", 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			0, 
			0, 
			Instance, 
			0);

        if (Window) {
            GlobalRunning = true;
        }
    }

    gl_info Info = {};
	if (!GLInit(&Info, Window)) {
		return -1;
	}

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontDefault();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(Window);
    ImGui_ImplOpenGL3_Init("#version 410");

    // Setup style
    ImGui::StyleColorsDark();

    bool show_demo_window = true;
    while (GlobalRunning) {
        MSG Message;
        while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
            switch (Message.message) {
			    case WM_QUIT: {
				    GlobalRunning = false;
			    } break;

			    default: {
				    TranslateMessage(&Message);
				    DispatchMessageA(&Message);
			    } break;
		    }
	        break;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Editor_Render();

        ImGui::Render(); 
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLSwapBuffers();
    }

    return 1;
}
