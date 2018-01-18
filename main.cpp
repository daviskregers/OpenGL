#include <QCoreApplication>
#include <iostream>
#include <glew/include/GL/glew.h>
#include <QDebug>

#include <display.h>
#include <shader.h>
#include <mesh.h>
#include <shader.h>
#include <texture.h>
#include <transform.h>
#include <camera.h>
#include <sdl2/include/SDL.h>
#include <beziersurface.h>
#include <subdivision.h>
#include <time.h>       /* time */

#define GLEW_BUILD
#define WIDTH 1440
#define HEIGHT 900

float random()
{
  float scale=RAND_MAX+1.;
  float base=rand()/scale;
  float fine=rand()/scale;
  return base+fine/scale;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    srand (time(NULL));

    Display display(WIDTH, HEIGHT, "OpenGL");
    Shader shader( a.applicationDirPath().toStdString() + "/res/basicShader");
    Shader shader_lights( a.applicationDirPath().toStdString() + "/res/basicShader");
    Shader shader_nolights( a.applicationDirPath().toStdString() + "/res/basicShader2");

    Texture bricks( a.applicationDirPath().toStdString() + "/res/bricks.jpg" );
    Texture rust( a.applicationDirPath().toStdString() + "/res/rust.jpg" );
    Texture rock( a.applicationDirPath().toStdString() + "/res/rock.jpg" );
    Texture watercolor( a.applicationDirPath().toStdString() + "/res/watercolor.jpg" );
    Texture red( a.applicationDirPath().toStdString() + "/res/red.jpg" );

    Transform transform;
    Transform transformBezier;
    Transform transformSubdiv;
    Transform transformText;
    Transform transformSurf;

    Mesh monkey( a.applicationDirPath().toStdString() + "/res/monkey3.obj" );
    Mesh bezier( a.applicationDirPath().toStdString() + "/res/bezier.obj" );

    Mesh subdiv( a.applicationDirPath().toStdString() + "/res/subdivision.obj" );
    Mesh text( a.applicationDirPath().toStdString() + "/res/text.obj" );

    std::vector<std::vector<Vertex>> points = {
                {
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(0,0)),
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(0,1)),
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(1,0)),
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(1,1))
                },
                {
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(0,0)),
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(0,1)),
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(1,0)),
                    Vertex(glm::vec3(random()*20,random()*20,random()*20), glm::vec2(1,1))
                },


    };

    BezierSurface surface(points, 1);
    surface.GenerateMesh(0, 1, 0, 1);
    Mesh surfaceMesh(surface.verts, surface.vertices.size(), surface.inds, surface.indices.size() );

    float counter = 0.0f;
    float counter_subd = 0.0f;
//    float c2 = -360.0f;

    display.numObjects = 3;
    display.numObject = 0;

    display.initTransforms();
    display.m_uiMouseX = 0;
    display.m_uiMouseY = 0;

    while(!display.isClosed()) {

        Camera camera(glm::vec3(display.offsetX,display.offsetY,(float)(-20.0f+display.Zoom)), 120.0f, (float)WIDTH/(float)HEIGHT, 0.001f, 10000.0f);
        camera.rotate(display.m_uiMouseX, display.m_uiMouseY);

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

        transformText.GetPos().x = 9;
        transformText.GetPos().y = 3;
        transformText.GetPos().z = -1;

        transformText.GetRot().x = 5;
        transformText.GetRot().y = 13;
        transformText.GetRot().z = 15.33;

        transformText.SetScale( glm::vec3(2,2,2) );

        transform.SetScale( scale );

        shader = (display.lights) ? shader_lights : shader_nolights;

        shader.Bind();
        watercolor.Bind(0);
        shader.Update(transformText, camera);
        text.Draw(GL_TRIANGLES);

        switch(display.numObject) {

            default:

                /*
                 * Monkey
                 */

                shader.Bind();
                bricks.Bind(0);
                shader.Update(transform, camera);
                monkey.Draw(GL_TRIANGLES);

            break;

//            case 1:

//                /*
//                 * Bezier
//                 */

//                transformBezier = transform;
//                shader.Bind();
//                rust.Bind(0);
//                shader.Update(transformBezier, camera);
//                bezier.Draw(GL_TRIANGLES);

//            break;

//            case 2:

//                /*
//                 * Subdivision
//                 */

//                transformSubdiv = transform;
//                shader.Bind();
//                rock.Bind(0);
//                shader.Update(transformSubdiv, camera);
//                subdiv.Draw(GL_TRIANGLES);

//            break;

            case 1:

            // Bezier Surface

                transformSurf = transformText;
                transformSurf.SetScale(glm::vec3(0.33, 0.33, 0.33));

                transformSurf.GetPos().x = 0;
                transformSurf.GetPos().y = 0;
                transformSurf.GetPos().z = 0;

                transformSurf.GetRot().x = 5;
                transformSurf.GetRot().y = 13;
                transformSurf.GetRot().z = 15.33;

                shader.Bind();
                rust.Bind(0);
                shader.Update(transformSurf, camera);

                surfaceMesh.Draw(GL_TRIANGLES);

            break;

        case 2:

        // Subdivision Surface

            glDisable(GL_CULL_FACE);


                transformSurf = transformText;
                transformSurf.SetScale(glm::vec3(0.5, 0.5, 0.5));

                transformSurf.GetPos().x = 0;
                transformSurf.GetPos().y = 0;
                transformSurf.GetPos().z = 0;

                shader.Bind();
                rock.Bind(0);
                shader.Update(transformSurf, camera);

              std::vector<Vertex> vertices;
              std::vector<unsigned int> indices;

              std::tie(vertices, indices)=make_spherified_cube_seams(counter_subd);

              Mesh m(&vertices[0], vertices.size(), &indices[0], indices.size() );
              m.Draw(GL_TRIANGLES);

              glEnable(GL_CULL_FACE);


            break;

        }


        /*
         * Swap buffers & update counter
         */

        display.SwapBuffers();
        if(display.transformPaused) {
            counter += 0.0001f * display.rotationSpeed;
            counter_subd = (counter_subd > 6) ? 0 : counter_subd + 0.0001f * abs(display.rotationSpeed);
            counter_subd = (abs(counter_subd) > 1000) ? 0 : counter_subd;
        }
//        c2 += 0.1f;

    }

    return 0;
}
