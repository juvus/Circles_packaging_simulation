/*=============================================================================
  Win32 platform with code for creation window and main program loop
  =============================================================================*/

/* Standard incudes: */
#include <windows.h>
#include <stdio.h>
#include <assert.h>

/* Program includes: */
#include <utils.h>
#include <simulation.h>
#include <input_treatment.h>

/* Timer parameters */
#define IDT_TIMER1 101
static UINT TIME_INTERVAL = 10;  /* Produce about xxx fps*/

/* Global variables */
static b32 is_running = true;  /* Flag for the game running indication */
static Render_Buffer_t render_buffer;  /* Buffer for the whole window image */
static Input_t user_input = {0};  /* Structure storing the user input data */
static LARGE_INTEGER begin_counter;  /* Variable for dtime calculation */
static LARGE_INTEGER end_counter;  /* Variable for dtime calculation */
static LARGE_INTEGER frequency_counter;  /* Variable for dtime calculation */
static f32 dtime = 0.01666f;  /* 60 fps first frame*/
static RECT rect;  /* Rectangle with game screen coordinates */
static HDC hdc = NULL;  /* Handle to the drawing content */
static MSG message = {0};  /* message that used in callback function */
static u32 vk_code;  /* Key virtual code */
static b32 was_down;  /* Flag that a button was down */
static b32 is_down;  /* Flag that a buttin is currently down */

/* Callback function definition */
static LRESULT CALLBACK
window_callback (HWND window, UINT message, WPARAM w_param, LPARAM l_param) {

    LRESULT result = 0;
    
    switch (message) {
    case WM_CLOSE:
    case WM_DESTROY: {
        is_running = false;
        break;
    }        
    case WM_CREATE: {
        /* Retrieves the coordinates of a window's client area */
        GetClientRect(window, &rect);
        render_buffer.width = (u32)(rect.right - rect.left);
        render_buffer.height = (u32)(rect.bottom - rect.top);
        
        /* Memory allocation with windows layer VirtualAlloc function */
        SIZE_T mem_size = sizeof(u32) * render_buffer.width * render_buffer.height;
        render_buffer.pixels = VirtualAlloc(0, mem_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

        /* Fill the bitmap_info */
        render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
        render_buffer.bitmap_info.bmiHeader.biWidth = render_buffer.width;
        render_buffer.bitmap_info.bmiHeader.biHeight = render_buffer.height;
        render_buffer.bitmap_info.bmiHeader.biPlanes = 1;
        render_buffer.bitmap_info.bmiHeader.biBitCount = 32;
        render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
		break;
    }
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP: {
        
        /* Getting the raw key codes */
        vk_code = (u32)w_param;
        was_down = ((l_param & (1 << 30)) != 0);
        is_down = ((l_param & (1 << 31)) == 0);
        
        /* Prepare the user input from the raw keyboard codes */
        reset_changed_state(&user_input);
        prepare_user_input(vk_code, was_down, is_down, &user_input);
    } break;

    case WM_TIMER: {
        /* Process the timer message */
        switch (w_param) {
        case IDT_TIMER1: {
            
            /* Simulation procedure */
            simulate_packaging(&user_input, dtime, &render_buffer);

            /* Render the calculated bitmap */
            StretchDIBits(hdc, 0, 0, (int)render_buffer.width, (int)render_buffer.height, 0, 0,
                          (int)render_buffer.width, (int)render_buffer.height, render_buffer.pixels,
                          &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

            /* Calculate the delta time */
            QueryPerformanceCounter(&end_counter);
            dtime = (f32)((end_counter.QuadPart - begin_counter.QuadPart) / (f32)frequency_counter.QuadPart);
            begin_counter = end_counter;
            
            break;
        }}
    } break;          
    default: {
            result = DefWindowProcA(window, message, w_param, l_param);
    }}
    return result;
}

/* Entry point for the windows application */
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    /* Avoid warning messages about not used function parameters */
    UNUSED(hPrevInstance);
    UNUSED(lpCmdLine);
    UNUSED(nShowCmd);
    
    /* Create a window class */
    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = window_callback;
    window_class.lpszClassName = "Simulation";

    /* Register the window class */
    RegisterClassA(&window_class);

    /* Create a window and return a window handle */
    DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU;
    HWND window = CreateWindowExA(0, window_class.lpszClassName, "Simulation", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0);
       
    /* Check if the window was successfully created */
    if (window == NULL) {
        assert(0 && "Window was not successfully created");
    }

    /* Get the drawing content (out created window) */
    hdc = GetDC(window);

    /* Set the timer */
    SetTimer(window, IDT_TIMER1, TIME_INTERVAL, (TIMERPROC)NULL);
        
    /* Window main loop */
    QueryPerformanceCounter(&begin_counter);
    QueryPerformanceFrequency(&frequency_counter);

    while ((GetMessage(&message, NULL, 0, 0)) && is_running) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    /* Free the memory */
    VirtualFree(render_buffer.pixels, 0, MEM_RELEASE);
    return 0;
}






    

