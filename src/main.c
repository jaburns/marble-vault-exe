//----------------------------------------------------------------------------------------------------------------------
// Forked from the iq/rgba 1k OpenGL intro system at https://www.iquilezles.org/code/isystem1k4k/isystem1k4k.htm
//----------------------------------------------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

#define XRES 1280
#define YRES  720

#include "shader.h"

#define oglCreateProgram        ((PFNGLCREATEPROGRAMPROC)        wglGetProcAddress("glCreateProgram"))
#define oglCreateShader         ((PFNGLCREATESHADERPROC)         wglGetProcAddress("glCreateShader"))
#define oglCreateShaderProgramv ((PFNGLCREATESHADERPROGRAMVPROC) wglGetProcAddress("glCreateShaderProgramv"))
#define oglShaderSource         ((PFNGLSHADERSOURCEPROC)         wglGetProcAddress("glShaderSource"))
#define oglCompileShader        ((PFNGLCOMPILESHADERPROC)        wglGetProcAddress("glCompileShader"))
#define oglAttachShader         ((PFNGLATTACHSHADERPROC)         wglGetProcAddress("glAttachShader"))
#define oglLinkProgram          ((PFNGLLINKPROGRAMPROC)          wglGetProcAddress("glLinkProgram"))
#define oglUseProgram           ((PFNGLUSEPROGRAMPROC)           wglGetProcAddress("glUseProgram"))
#define oglGetShaderInfoLog     ((PFNGLGETSHADERINFOLOGPROC)     wglGetProcAddress("glGetShaderInfoLog"))
#define oglUniform4iv           ((PFNGLUNIFORM4IVPROC)           wglGetProcAddress("glUniform4iv"))
#define oglGetUniformLocation   ((PFNGLGETUNIFORMLOCATIONPROC)   wglGetProcAddress("glGetUniformLocation"))

const static PIXELFORMATDESCRIPTOR pfd = {
    0, 0, PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER, 0, 0, 0, 0, 0, 0, 0, 0,
    8, // alpha channel bits
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#ifdef _DEBUG
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#else
void entrypoint( void )
#endif
{ 
    HDC hDC = GetDC( CreateWindow( "edit", 0, WS_POPUP|WS_VISIBLE,
        GetSystemMetrics(SM_CXSCREEN)/2-640,
        GetSystemMetrics(SM_CYSCREEN)/2-360,
        1280,
        720,
        0,0,0,0
    ));

    SetPixelFormat( hDC, ChoosePixelFormat( hDC, &pfd ), &pfd );
    wglMakeCurrent( hDC, wglCreateContext( hDC ));

#ifdef _DEBUG

    GLchar infolog[1000];
    GLsizei infolen;
    int program = oglCreateProgram();
    int f = oglCreateShader( GL_FRAGMENT_SHADER );	
    oglShaderSource( f, 1, &shader_glsl, 0 );
    oglCompileShader( f );
    oglGetShaderInfoLog( f, 1000, &infolen, infolog );
    oglAttachShader( program, f );
    oglLinkProgram( program );
    oglUseProgram( program );
    OutputDebugString( infolog );

#else

    int program = oglCreateShaderProgramv( GL_FRAGMENT_SHADER, 1, &shader_glsl );
    oglUseProgram( program );

#endif

    int lastKeys[128];
    int curKeys[128];
    int edgeKeys[128];

    int track = 1;

    for(;;)
    {
        int g[] = {
            0, 0, 0, 0,
            128, 1, 128, 1,
            255, 1, 192, 1,
            0, 100, 0, 7
        };
        unsigned char buffer[16];

        int frames = 0;
        int ballPos = 0;
        int cameraOffset = 0;
        int camFromBall = 0;

        for(;;)
        {
            memcpy( lastKeys, curKeys, 128*sizeof(int) );
            for( int i = 0; i < 128; ++i ) {
                curKeys[i] = GetAsyncKeyState(i);
                edgeKeys[i] = curKeys[i] && !lastKeys[i] ? 1 : 0;
            }

            if( edgeKeys[ 'R' ]) break;
            if( track < 10 && edgeKeys[ 'N' ] ) { track++; break; }
            if( track > 1  && edgeKeys[ 'P' ] ) { track--; break; }
            if( edgeKeys[ VK_ESCAPE ]) ExitProcess( 0 );

            g[0] = track;
            g[1] = cameraOffset;
            g[2] = edgeKeys[VK_UP] + 2*edgeKeys[VK_DOWN];

            int ballVelX = (256*g[8] + g[9]) - 32768;
            ballPos += ballVelX / 70;
            camFromBall += (ballVelX / 3 - camFromBall) / 99;
            cameraOffset = camFromBall + ballPos;
            frames++;

            g[3] = cameraOffset;

            oglUniform4iv( oglGetUniformLocation( program, "g" ), 4, g );

            glRects( -1, -1, 1, 1 );

            glReadPixels( 0, 0, 4, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
            for( int i = 0; i < 16; ++i )
                g[i] = buffer[i];

            SwapBuffers( hDC );

            if( ballPos > 10 * 32768 )
            {
                char fmt[64];
                wsprintfA(fmt, "Track %d: %d.%02d", track, frames / 60, 100 * (frames % 60) / 60);
                MessageBoxA(0, fmt, "", 0);
                if (++track > 10) track = 1;
                break;
            }
        }
    }

    ExitProcess( 0 );
}
