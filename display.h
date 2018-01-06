#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <sdl2/include/SDL.h>

class Display
{
public:
    Display(int width, int height, const std::string& title);
    void SwapBuffers();
    void Clear( float r, float g, float b, float a );
    bool isClosed();
    virtual ~Display();

    unsigned int numObjects;
    unsigned int numObject;
    float Zoom;
    float rotationSpeed;
    bool transformPaused;

    unsigned int width;
    unsigned int height;

    float offsetX, offsetY;


    float m_uiMouseX, m_uiMouseY;
    bool mouseDown;

    void initTransforms();

protected:
private:

    SDL_Window* m_window;
    SDL_GLContext m_glContext;

    bool m_isClosed;
};

#endif // DISPLAY_H
