#include "Transform.hpp"

Transform::Transform()
:posX(0.f),posY(0.f),posZ(0.f),
scaleX(1.f),scaleY(1.f),scaleZ(1.f),
rotX(0.0f),rotY(0.0f),rotZ(0.0f),
keyX(0.0), keyY(0.0), keyZ(0.0)
{
    mxTransform = glm::mat4();
    parent = nullptr;
}

Transform::~Transform(){}

glm::mat4 Transform::getLocalTransformMatrix() {
    if(!needUpdate)
        return mxTransform;
    mxTransform = glm::mat4();
    mxTransform = glm::translate(mxTransform, glm::vec3(posX, posY, posZ));
    mxTransform = glm::scale(mxTransform, glm::vec3(scaleX, scaleY, scaleZ));
    glm::quat quatKey(glm::vec3(keyX, keyY, keyZ));
    keyX=keyY=keyZ=0.0f;
    quatStorage = quatKey*quatStorage;
    quatStorage = glm::normalize(quatStorage);
    needUpdate=false;
    mxTransform = mxTransform*glm::toMat4(quatStorage);
    return mxTransform;
}

glm::mat4 Transform::getGlobalTransformMatrix() {
    glm::mat4 mx = getLocalTransformMatrix();
    if(parent == nullptr) {
        return getLocalTransformMatrix();
    }
    else {
        return parent->getGlobalTransformMatrix() * getLocalTransformMatrix();
    }
    return mx;
}

void Transform::translate(float dx, float dy, float dz) {
    posX += dx;
    posY += dy;
    posZ += dz;
    needUpdate=true;
}

void Transform::translate(glm::vec3 vec) {
    posX += vec.x;
    posY += vec.y;
    posZ += vec.z;
    needUpdate = true;
}

void Transform::rotate(float dx, float dy, float dz) {
    keyX += dx;
    keyY += dy;
    keyZ += dz;

    rotX += dx;
    rotY += dy;
    rotZ += dz;
    needUpdate=true;
}

void Transform::rotate(glm::vec3 vec) {
    keyX += vec.x;
    keyY += vec.y;
    keyZ += vec.z;

    rotX += vec.x;
    rotY += vec.y;
    rotZ += vec.z;
    needUpdate = true;
}

void Transform::scaleUp(float dx, float dy, float dz) {
    scaleX += dx;
    scaleY += dy;
    scaleZ += dz;
    needUpdate = true;
}

void Transform::scaleUp(glm::vec3 vec) {
    scaleX += vec.x;
    scaleY += vec.y;
    scaleZ += vec.z;
    needUpdate = true;
}

void Transform::setPosition(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
    needUpdate=true;
}

void Transform::setPosition(glm::vec3 vec) {
    posX = vec.x;
    posY = vec.y;
    posZ = vec.z;
    needUpdate = true;
}

void Transform::setRotation(float x, float y, float z) {
    keyX += x-rotX;
    keyY += y-rotY;
    keyZ += z-rotZ;

    rotX = x;
    rotY = y;
    rotZ = z;
    needUpdate = true;
}

void Transform::setRotation(glm::vec3 vec) {
    keyX += vec.x-rotX;
    keyY += vec.y-rotY;
    keyZ += vec.z-rotZ;

    rotX = vec.x;
    rotY = vec.y;
    rotZ = vec.z;
    needUpdate = true;
}

void Transform::setScale(float dx, float dy, float dz) {
    scaleX = dx;
    scaleY = dy;
    scaleZ = dz;
    needUpdate=true;
}

void Transform::setScale(glm::vec3 vec) {
    scaleX = vec.x;
    scaleY = vec.y;
    scaleZ = vec.z;
    needUpdate=true;
}

void Transform::setParent(Transform* t) {
    parent = t;
    t->vecChildren.push_back(this);
}

glm::vec3 Transform::getFront(){
    glm::vec3 vFront(0,0,1);
    vFront = glm::rotate(vFront, rotX, glm::vec3(1,0,0));
    vFront = glm::rotate(vFront, rotY, glm::vec3(0,1,0));
    vFront = glm::rotate(vFront, rotZ, glm::vec3(0,0,1));
    return -glm::normalize(vFront);
}

glm::vec3 Transform::getRight(){
    glm::vec3 vRight(1,0,0);
    vRight = glm::rotate(vRight, rotX, glm::vec3(1,0,0));
    vRight = glm::rotate(vRight, rotY, glm::vec3(0,1,0));
    vRight = glm::rotate(vRight, rotZ, glm::vec3(0,0,1));
    return glm::normalize(vRight);
}

glm::vec3 Transform::getUp(){
    glm::vec3 vUp(0,1,0);
    vUp = glm::rotate(vUp, rotX, glm::vec3(1,0,0));
    vUp = glm::rotate(vUp, rotY, glm::vec3(0,1,0));
    vUp = glm::rotate(vUp, rotZ, glm::vec3(0,0,1));
    return glm::normalize(vUp);
}

void Transform::rotateX(float dx){
    keyX += dx;
    rotX+=dx;needUpdate=true;
}

void Transform::rotateY(float dy){
    keyY += dy;
    rotY+=dy;needUpdate=true;
}

void Transform::rotateZ(float dz){
    keyZ += dz;
    rotZ+=dz;needUpdate=true;
}

void Transform::moveX(float dx){
    posX+=dx;needUpdate=true;
}

void Transform::moveY(float dy){
    posY+=dy;needUpdate=true;
}

void Transform::moveZ(float dz){
    posZ+=dz;needUpdate=true;
}

void Transform::scaleUpX(float dx){
    scaleX += dx; needUpdate=true;
}

void Transform::scaleUpY(float dy){
    scaleY += dy; needUpdate=true;
}

void Transform::scaleUpZ(float dz){
    scaleZ += dz; needUpdate=true;
}
/* -###- getters -###- */
float Transform::getX(){
    return posX;
}

float Transform::getY(){
    return posY;
}

float Transform::getZ(){
    return posZ;
}

float Transform::getScaleX(){
    return scaleX;
}

float Transform::getScaleY(){
    return scaleY;
}

float Transform::getScaleZ(){
    return scaleZ;
}

float Transform::getRotX(){
    return rotX;
}

float Transform::getRotY(){
    return rotY;
}

float Transform::getRotZ(){
    return rotZ;
}
/* -###- setters -###- */
void Transform::setX(float x){
    posX=x;needUpdate=true;
}

void Transform::setY(float y){
    posY=y;needUpdate=true;
}

void Transform::setZ(float z){
    posZ=z;needUpdate=true;
}

void Transform::setScaleX(float x){
    scaleX=x;needUpdate=true;
}

void Transform::setScaleY(float y){
    scaleY=y;needUpdate=true;
}

void Transform::setScaleZ(float z){
    scaleZ=z;needUpdate=true;
}

void Transform::setRotX(float x){
    keyX += x-rotX;
    rotX=x;needUpdate=true;
}

void Transform::setRotY(float y){
    keyY += y-rotY;
    rotY=y;needUpdate=true;
}

void Transform::setRotZ(float z){
    keyZ += z-rotZ;
    rotZ=z;needUpdate=true;
}

glm::vec3 Transform::getPosition(){
    return glm::vec3(posX,posY,posZ);
}

glm::vec3 Transform::getRotation(){
    return glm::vec3(rotX,rotY,rotZ);
}

glm::vec3 Transform::getScale()   {
    return glm::vec3(scaleX,scaleY,scaleZ);
}

void Transform::print(){
    std::cout << glm::to_string(mxTransform) << "\n";
}

Transform* Transform::getParent(){
    return parent;
}
