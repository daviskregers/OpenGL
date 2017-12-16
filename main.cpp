#include <QCoreApplication>
#include <iostream>
#include <glew/include/GL/glew.h>

#include <display.h>
#include <shader.h>
#include <mesh.h>
#include <shader.h>
#include <texture.h>
#include <transform.h>
#include <camera.h>

#define GLEW_BUILD
#define WIDTH 1920
#define HEIGHT 1080

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Display display(WIDTH, HEIGHT, "OpenGL");
    Shader shader( a.applicationDirPath().toStdString() + "/res/basicShader");
    Texture texture( a.applicationDirPath().toStdString() + "/res/bricks.jpg" );
    Transform transform;

//    Vertex vertices[] =  {
//        Vertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0)),
//        Vertex(glm::vec3(0,0.5,0), glm::vec2(0.5,1.0)),
//        Vertex(glm::vec3(0.5,-0.5,0), glm::vec2(1.0,0.0)),
//    };
//    unsigned int indices[] = {0, 1, 2};

//    Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]) );
    Mesh mesh2( a.applicationDirPath().toStdString() + "/res/monkey3.obj" );

    float counter = 0.0f;

    Camera camera(glm::vec3(0,0,-10), 70.0f, (float)WIDTH/(float)HEIGHT, 0.01f, 1000.0f);

    while(!display.isClosed()) {

        display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

        float sinCounter = sinf(counter);
        float cosCounter = cosf(counter);

        transform.GetPos().x = sinCounter;
        transform.GetPos().z = cosCounter;
        transform.GetRot().x = counter * 5;
        transform.GetRot().y = counter * 5;
        transform.GetRot().z = counter * 5;
//        transform.SetScale(glm::vec3(cosCounter, cosCounter, cosCounter));

        shader.Bind();
        texture.Bind(0);
        shader.Update(transform, camera);
//        mesh.Draw();
        mesh2.Draw();

        display.SwapBuffers();

        counter += 0.0001f;

    }

    return 0;
}
