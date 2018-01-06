#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

class Camera
{
public:

    Camera(const glm::vec3 pos, float fov, float aspect, float zNear, float zFar)
    {

        m_perspective = glm::perspective(fov, aspect, zNear, zFar);
        m_position = pos;

        m_rot = glm::mat4(1.0f);

        m_forward = glm::vec3(0,0,1);
        m_up = glm::vec3(0,1,0);

    }

    void rotate(float x, float y, float z = 0) {

        m_rot = glm::rotate(m_rot, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_rot = glm::rotate(m_rot, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_rot = glm::rotate(m_rot, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));

    }

    inline glm::mat4 GetViewProjection() const
    {
        return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up) * m_rot;
    }

private:

    glm::mat4 m_perspective;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::mat4 m_rot;

};

#endif // CAMERA_H
