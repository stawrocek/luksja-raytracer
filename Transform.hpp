#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <vector>

/*
    Axis x -> from left(-) to right(+)
    Axis y -> from bottom(-) to top(+)
    Axis z -> from to user(-) to screen(+)
*/

enum Space{
    LOCAL = 1,
    GLOBAL = 2
};

class Transform
{
public:
    Transform();
		~Transform();

    /*get transform in local space*/
    glm::mat4 getLocalTransformMatrix();
    /*get transform in global space (scene)*/
    glm::mat4 getGlobalTransformMatrix();

    void setParent(Transform* t);

    void setPosition(float x, float y, float z=0.f);
    void setPosition(glm::vec3);
    void setRotation(float x, float y, float z);
    void setRotation(glm::vec3);
    void setScale(float dx, float dy, float dz=1.0f);
    void setScale(glm::vec3);

    /* -###- move by d -###- */
    void translate(float dx, float dy, float dz=0.f);
    void translate(glm::vec3);
    void rotate(float dx, float dy, float dz=0.f);
    void rotate(glm::vec3);
    void scaleUp(float dx, float dy, float dz=0.f);
    void scaleUp(glm::vec3);

    void rotateX(float dx);
    void rotateY(float dy);
    void rotateZ(float dz);
    void moveX(float dx);
    void moveY(float dy);
    void moveZ(float dz);
    void scaleUpX(float dx);
    void scaleUpY(float dy);
    void scaleUpZ(float dz);
    /* -###- getters -###- */
    float getX();
    float getY();
    float getZ();
    float getScaleX();
    float getScaleY();
    float getScaleZ();
    float getRotX();
    float getRotY();
    float getRotZ();
    /* -###- setters -###- */
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    void setRotX(float x);
    void setRotY(float y);
    void setRotZ(float z);

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    glm::vec3 getFront();   //z
    glm::vec3 getRight();   //x
    glm::vec3 getUp();      //y

    Transform* getParent();

    void print();

protected:
    glm::mat4 mxTransform;
    bool needUpdate=false;
    float posX, posY, posZ;
    float scaleX, scaleY, scaleZ;
    float rotX, rotY, rotZ;
    Transform* parent;
    float keyX, keyY, keyZ;
    glm::quat quatStorage;
    std::vector<Transform* > vecChildren;
};

#endif // TRANSFORM_HPP

