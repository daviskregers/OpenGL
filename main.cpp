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
#define WIDTH 1440
#define HEIGHT 900

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Display display(WIDTH, HEIGHT, "OpenGL");
    Shader shader( a.applicationDirPath().toStdString() + "/res/basicShader");

    Texture bricks( a.applicationDirPath().toStdString() + "/res/bricks.jpg" );
    Texture rust( a.applicationDirPath().toStdString() + "/res/rust.jpg" );
    Texture rock( a.applicationDirPath().toStdString() + "/res/rock.jpg" );

    Transform transform;
    Transform transformBezier;
    Transform transformSubdiv;

//    Vertex vertices[] =  {
//        Vertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0)),
//        Vertex(glm::vec3(0,0.5,0), glm::vec2(0.5,1.0)),
//        Vertex(glm::vec3(0.5,-0.5,0), glm::vec2(1.0,0.0)),
//    };
//    unsigned int indices[] = {0, 1, 2};

//    Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]) );
    Mesh monkey( a.applicationDirPath().toStdString() + "/res/monkey3.obj" );
    Mesh bezier( a.applicationDirPath().toStdString() + "/res/bezier.obj" );
    Mesh subdiv( a.applicationDirPath().toStdString() + "/res/subdivision.obj" );
    float counter = 0.0f;

    Camera camera(glm::vec3(0,0,-20), 120.0f, (float)WIDTH/(float)HEIGHT, 0.01f, 1000.0f);

    while(!display.isClosed()) {

        display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

        float sinCounter = sinf(counter);
        float cosCounter = cosf(counter);

        transform.GetPos().x = sinCounter;
        transform.GetPos().z = cosCounter;
        transform.GetRot().x = counter * 5;
        transform.GetRot().y = counter * 5;
        transform.GetRot().z = counter * 5;

        glm::vec3 scale(
            (cosCounter < 0.5) ? 0.5 : cosCounter,
            (cosCounter < 0.5) ? 0.5 : cosCounter,
            (cosCounter < 0.5) ? 0.5 : cosCounter
        );

        transform.SetScale( scale );

        /*
         * Monkey
         */

        shader.Bind();
        bricks.Bind(0);
        shader.Update(transform, camera);
        monkey.Draw();

        /*
         * Bezier
         */

        transformBezier = transform;

        transformBezier.GetPos().x += 5;
        transformBezier.GetPos().y += 5;
        transformBezier.GetPos().z = 9;

        shader.Bind();
        rust.Bind(0);
        shader.Update(transformBezier, camera);
        bezier.Draw();

        /*
         * Subdivision
         */

        transformSubdiv = transform;

        transformSubdiv.GetPos().x -= 5;
        transformSubdiv.GetPos().y -= 5;
        transformSubdiv.GetPos().z = 9;

        shader.Bind();
        rock.Bind(0);
        shader.Update(transformSubdiv, camera);
        subdiv.Draw();

        /*
         * Swap buffers & update counter
         */

        display.SwapBuffers();
        counter += 0.0001f;

    }

    return 0;
}
