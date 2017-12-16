#include "display.h"

#define GLEW_STATIC
#include <glew/include/GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title)
{

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // 8 biti krāsai
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    m_glContext = SDL_GL_CreateContext(m_window); // nodrošina GPU izmantošanu renderēšanas procesam

    GLenum status = glewInit(); // atrod visas funkcijas, kuras sistēma atbalsta

    if(status != GLEW_OK) {
        std::cerr << "Glew failed to initialize" << std::endl;
    }

    m_isClosed = false;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

bool Display::isClosed() {
    return m_isClosed;
}

void Display::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}

void Display::SwapBuffers() {
    SDL_GL_SwapWindow(m_window); // Window rāda 1 logu, kamēr OpenGL zīmē otru

    SDL_Event event;

    while( SDL_PollEvent( &event ) ){

        switch( event.type ){

          case SDL_QUIT:
            m_isClosed = true;
            break;

          case SDL_KEYDOWN:

                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        m_isClosed = true;
                    break;
                }

            break;

          case SDL_KEYUP:
          break;

          default:
          break;

        }
    }
}

Display::~Display()
{

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    delete m_window;

}
