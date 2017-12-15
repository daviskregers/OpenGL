#include <QCoreApplication>
#include <iostream>
#include <glew/include/GL/glew.h>

#include <display.h>
#include <shader.h>
#include <mesh.h>
#include <shader.h>
#include <texture.h>

#define GLEW_BUILD

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Display display(800, 600, "OpenGL");
    Shader shader( a.applicationDirPath().toStdString() + "/res/basicShader");
    Texture texture( a.applicationDirPath().toStdString() + "/res/bricks.jpg" );

    Vertex vertices[] =  {
        Vertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0)),
        Vertex(glm::vec3(0,0.5,0), glm::vec2(0.5,1.0)),
        Vertex(glm::vec3(0.5,-0.5,0), glm::vec2(1.0,0.0)),
    };


    Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]) );

    while(!display.isClosed()) {

        display.Clear(0.0f, 1.0f, 0.0f, 1.0f);
        shader.Bind();
        texture.Bind(0);
        mesh.Draw();

        display.SwapBuffers();

    }

    return 0;
}
