#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include "glext.h"

#define XRES 1280
#define YRES  720

#include "shader.h"

//extern "C" int _fltused = 0;

const static PIXELFORMATDESCRIPTOR pfd = {0,0,PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#ifdef _DEBUG

const static char *funcNames[] = {
    "glCreateProgram",
    "glCreateShader",
    "glShaderSource",
    "glCompileShader",
    "glAttachShader",
    "glLinkProgram",
    "glUseProgram",
    "glGetShaderInfoLog"
};

static void *funcPtrs[8];

#define oglCreateProgram    ((PFNGLCREATEPROGRAMPROC)funcPtrs[0])
#define oglCreateShader     ((PFNGLCREATESHADERPROC)funcPtrs[1])
#define oglShaderSource     ((PFNGLSHADERSOURCEPROC)funcPtrs[2])
#define oglCompileShader    ((PFNGLCOMPILESHADERPROC)funcPtrs[3])
#define oglAttachShader     ((PFNGLATTACHSHADERPROC)funcPtrs[4])
#define oglLinkProgram      ((PFNGLLINKPROGRAMPROC)funcPtrs[5])
#define oglUseProgram       ((PFNGLUSEPROGRAMPROC)funcPtrs[6])
#define oglGetShaderInfoLog ((PFNGLGETSHADERINFOLOGPROC)funcPtrs[7])

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{ 
    RECT rec = { 0, 0, XRES, YRES };
    WNDCLASS wc;

    ZeroMemory( &wc, sizeof(WNDCLASS) );
    wc.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hPrevInstance;
    wc.lpszClassName = "jb";
    RegisterClass( &wc );

    const int dws = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    AdjustWindowRect( &rec, dws, 0 );
    HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, wc.lpszClassName, "win32game-2k", dws,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, hPrevInstance, 0 );
    HDC hDC = GetDC( hWnd );

#else

void entrypoint( void )
{ 
    DEVMODE screenSettings = {{0},0,0,156,0,0x001c0000,{0},0,0,0,0,0,{0},0,32,XRES,YRES,{0},0,0,0,0,0,0,0,0,0};
    ChangeDisplaySettings( &screenSettings, CDS_FULLSCREEN );
    ShowCursor( 0 );
    HDC hDC = GetDC( CreateWindow( "edit", 0, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 0,0,0,0,0,0,0,0 ));

#endif

    SetPixelFormat( hDC, ChoosePixelFormat( hDC, &pfd ), &pfd );
    wglMakeCurrent( hDC, wglCreateContext( hDC ));

#ifdef _DEBUG

    for( int i = 0; i < (sizeof(funcPtrs)/sizeof(void*)); i++ )
        funcPtrs[i] = wglGetProcAddress( funcNames[i] );

    GLchar infolog[1000];
    GLsizei infolen;
    int p = oglCreateProgram();
    int f = oglCreateShader(GL_FRAGMENT_SHADER);	
    oglShaderSource(f, 1, &shader_glsl, 0);
    oglCompileShader(f);
    oglGetShaderInfoLog(f, 1000, &infolen, infolog);
    oglAttachShader(p,f);
    oglLinkProgram(p);
    oglUseProgram(p);

#else

    ((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))(
        ((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress("glCreateShaderProgramv"))( 
            GL_FRAGMENT_SHADER, 1, &shader_glsl
        )
    );

#endif

    do { 
        glColor3us( (unsigned short)GetTickCount(), GetAsyncKeyState( VK_UP ), 0 );
        glRects( -1, -1, 1, 1 );
        SwapBuffers( hDC );
    }
    while( !GetAsyncKeyState( VK_ESCAPE ) );

    ExitProcess( 0 );
}
