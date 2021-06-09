#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef CAMERA_H
#define CAMERA_H

#define _CAMERA_UP 0x01
#define _CAMERA_DOWN 0x02
#define _CAMERA_LEFT 0x04
#define _CAMERA_RIGHT 0x08
#define _CAMERA_FORWARD 0x10
#define _CAMERA_BACKWARD 0x20

const float ZOOM = 45.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float SPEED = 2.5f;

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 left;
    glm::vec3 up;
    glm::vec3 world_up;
    float lastx, lasty;
    float yaw, pitch;
    float zoomlevel;
    float movement_speed;
    float mouse_sensitivity;
    int width, height;

    void updateCameraVectors()
    {
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::cross(right, front);
    }

public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float pitch = PITCH, float yaw = YAW, float zoomlevel = ZOOM, float sensitivity = SENSITIVITY, float speed = SPEED) : world_up(glm::vec3(0.0f, 1.0f, 0.0f)), width(800), height(600)
    {

        position = pos;
        this->pitch = pitch;
        this->yaw = yaw;
        this->zoomlevel = zoomlevel;
        movement_speed = speed;
        mouse_sensitivity = sensitivity;
        updateCameraVectors();
        lastx = (float)width / 2;
        lasty = (float)height / 2;
    }
    Camera(int width, int height, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float pitch = PITCH, float yaw = YAW, float zoomlevel = ZOOM, float sensitivity = SENSITIVITY, float speed = SPEED) : world_up(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        position = pos;
        this->pitch = pitch;
        this->yaw = yaw;
        this->zoomlevel = zoomlevel;
        movement_speed = speed;
        mouse_sensitivity = sensitivity;
        updateCameraVectors();
        lastx = (float)width / 2;
        lasty = (float)height / 2;
    };
    Camera(float posX, float posY, float posZ, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float pitch = PITCH, float yaw = YAW) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoomlevel(ZOOM), lastx(400), lasty(300)
    {
        position = glm::vec3(posX, posY, posZ);
        world_up = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }
    ~Camera(){

    };
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }
    glm::mat4 GetViewMatrix()
    {
        return getViewMatrix();
    }
    void move(int direction, float delta)
    {
        float speed = movement_speed * delta;
        if (_CAMERA_UP & direction)
        {
            position += speed * up;
        }
        else if (_CAMERA_DOWN & direction)
        {
            position -= speed * up;
        }
        if (_CAMERA_RIGHT & direction)
        {
            position += speed * right;
        }
        else if (_CAMERA_LEFT & direction)
        {
            position -= speed * right;
        }
        if (_CAMERA_BACKWARD & direction)
        {
            position -= speed * front;
        }
        else if (_CAMERA_FORWARD & direction)
        {
            position += speed * front;
        }
    }
    void zoom(float step)
    {
        zoomlevel += step;
        if (zoomlevel < 1.0f)
        {
            zoomlevel = 1.0f;
        }
        else if (zoomlevel > 45.0f)
        {
            zoomlevel = 45.0f;
        }
    }
    void turn(float xpos, float ypos, GLboolean constraint = GL_TRUE)
    {
        float xoffset = (xpos - lastx) * mouse_sensitivity;
        float yoffset = (lasty - ypos) * mouse_sensitivity;
        yaw += xoffset;
        pitch += yoffset;
        lastx = xpos;
        lasty = ypos;
        if (constraint)
        {
            if (pitch > 89.0f)
            {
                pitch = 89.0f;
            }
            else if (pitch < -89.0f)
            {
                pitch = -89.f;
            }
        }
        updateCameraVectors();
    }
    void ProcessMouseScroll(float ypos)
    {
    }
    void ProcessMouseMovement(float xpos, float ypos, GLboolean constraint = GL_TRUE)
    {
        turn(xpos, ypos, constraint);
    }
    void ProcessKeyboardInput(int direction, float delta)
    {
        move(direction, delta);
    }
    float getZoom()
    {
        return zoomlevel;
    }
    glm::vec3 getPosition()
    {
        return position;
    }
};
#endif
