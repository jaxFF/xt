#define ERROR_INVALID_VERSION_ARB               0x2095
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB            0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126


#define WGL_NUMBER_PIXEL_FORMATS_ARB            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                  0x2002
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_NEED_PALETTE_ARB                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB              0x2006
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                0x2009
#define WGL_TRANSPARENT_ARB                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB         0x203B
#define WGL_SHARE_DEPTH_ARB                     0x200C
#define WGL_SHARE_STENCIL_ARB                   0x200D
#define WGL_SHARE_ACCUM_ARB                     0x200E
#define WGL_SUPPORT_GDI_ARB                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_STEREO_ARB                          0x2012
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_RED_SHIFT_ARB                       0x2016
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_GREEN_SHIFT_ARB                     0x2018
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_BLUE_SHIFT_ARB                      0x201A
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_ALPHA_SHIFT_ARB                     0x201C
#define WGL_ACCUM_BITS_ARB                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                0x2021
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_AUX_BUFFERS_ARB                     0x2024
#define WGL_NO_ACCELERATION_ARB                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB            0x2026
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                  0x202A

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                 0x202C

typedef HGLRC WINAPI wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hDC, const int* piAttribIList, const float* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

global wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
global wgl_choose_pixel_format_arb* wglChoosePixelFormatARB;
global HDC WindowDC;
global int OpenGLAttribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 0,
	WGL_CONTEXT_FLAGS_ARB, 0,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0,
	0
};

LRESULT CALLBACK Win32FakeWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
	LRESULT Result = 0;

	switch (Message) {
    case WM_ACTIVATEAPP:
    case WM_DESTROY:
	case WM_PAINT:
    case WM_CLOSE: {
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

    default: {
        Result = DefWindowProcA(Window, Message, wParam, lParam);
    } break;
	}

	return Result;
}

void SetPixelFormat(HDC WindowDC) {
	int PixelFormatIndex = 0;
	UINT NumFormats;
	if (wglChoosePixelFormatARB) {
		int Attribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0,
		};

		f32 FloatAttribs[] = {0};

		if (wglChoosePixelFormatARB(WindowDC, Attribs, FloatAttribs, 1, &PixelFormatIndex, &NumFormats) == FALSE) {
			printf("SetPixelFormat: wglChoosePixelFormatARB failed!");
			return;
		}
	}

	{
		PIXELFORMATDESCRIPTOR PixelFormat;
		PixelFormat.nSize = sizeof(PixelFormat);
		PixelFormat.nVersion = 1;
		PixelFormat.iPixelType = PFD_TYPE_RGBA;
#if 1
		PixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
#else
		PixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
#endif
		PixelFormat.cColorBits = 32;
		PixelFormat.cAlphaBits = 8;
		PixelFormat.iLayerType = PFD_MAIN_PLANE;
		PixelFormatIndex = ChoosePixelFormat(WindowDC, &PixelFormat);
	}

	// Set our pixel format and create a context for real this time
	PIXELFORMATDESCRIPTOR PixelFormat;
	//PixelFormatIndex = ChoosePixelFormat(WindowDC, &PixelFormat);
	DescribePixelFormat(WindowDC, PixelFormatIndex, sizeof(PixelFormat), &PixelFormat);
	SetPixelFormat(WindowDC, PixelFormatIndex, &PixelFormat);
}

void WGLLoadExtensions() {
	char* FakeName = "GL_WGLLoader";
	WNDCLASSEXA FakeWindowClass = { };
	FakeWindowClass.cbSize = sizeof(WNDCLASSEXA);
	FakeWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	FakeWindowClass.lpfnWndProc = (WNDPROC)Win32FakeWindowCallback;
	FakeWindowClass.cbClsExtra = 0;
	FakeWindowClass.cbWndExtra = 0;
	FakeWindowClass.hInstance = GetModuleHandleA(NULL);
	FakeWindowClass.hIcon = LoadIconA(NULL, IDI_WINLOGO);
	FakeWindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	FakeWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	FakeWindowClass.lpszMenuName = nullptr;
	FakeWindowClass.lpszClassName = FakeName;
	FakeWindowClass.hIconSm = LoadIconA(NULL, IDI_WINLOGO);

	if (RegisterClassExA(&FakeWindowClass)) {
		HWND FakeWindow = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			FakeName,
			FakeName,
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			0,
			0,
			NULL,
			NULL,
			GetModuleHandleA(NULL),
			NULL);	

		HDC FakeWindowDC = GetDC(FakeWindow);
		SetPixelFormat(FakeWindowDC);
		HGLRC FakeOpenGLRC = wglCreateContext(FakeWindowDC);
		if (wglMakeCurrent(FakeWindowDC, FakeOpenGLRC)) {
			wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb*)wglGetProcAddress("wglChoosePixelFormatARB");
			wglCreateContextAttribsARB = (wgl_create_context_attribs_arb*)wglGetProcAddress("wglCreateContextAttribsARB");

			wglMakeCurrent(0, 0);
		}

		wglDeleteContext(FakeOpenGLRC);
		ReleaseDC(FakeWindow, FakeWindowDC);
		DestroyWindow(FakeWindow);
		UnregisterClassA(FakeName, GetModuleHandleA(NULL));
	}
}

bool GLInitContext(void* _Handle) {
	HWND Handle = (HWND)_Handle;
	WindowDC = GetDC(Handle);
	WGLLoadExtensions();

	HGLRC OpenGLRC = 0;
	if (wglCreateContextAttribsARB) {
		SetPixelFormat(WindowDC);
		OpenGLRC = wglCreateContextAttribsARB(WindowDC, 0, OpenGLAttribs); 
	}

	if (!OpenGLRC) {
		OpenGLRC = wglCreateContext(WindowDC); 
	}

	if (wglMakeCurrent(WindowDC, OpenGLRC)) {
		// note: We failed to get wglSwapLayerBuffers, so fallback!
		//if (!wglSwapLayerBuffers) {
			// note: Try grabbing wglSwapLayerBuffers directly from opengl32.dll
			HMODULE GLLibrary = (HMODULE)LoadLibraryExA("opengl32.dll", nullptr, 0);
			//wglSwapLayerBuffers = (wgl_swap_layer_buffers*)GetProcAddress(GLLibrary, "wglSwapLayerBuffers");
			FreeLibrary(GLLibrary);

			// note: If the prior efforts fail, grab SwapBuffers instead
			//if (!wglSwapLayerBuffers) {
				HMODULE GDILibrary = (HMODULE)LoadLibraryExA("gdi32.dll", nullptr, 0);
				//SwapBuffers = (swap_buffers*)GetProcAddress(GDILibrary, "SwapBuffers");
				FreeLibrary(GDILibrary);
			//}
		//};

		printf("Sucessfully initalized WGL context\n");
		return true;
	}

	ReleaseDC(Handle, WindowDC);
	return false;
}

bool GLSwapBuffers() {
	//if (!wglSwapLayerBuffers)
		//return SwapBuffers(WindowDC);
	//else
		return wglSwapLayerBuffers(WindowDC, WGL_SWAP_MAIN_PLANE);
}