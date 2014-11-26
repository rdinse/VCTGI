#define GLM_SWIZZLE
#include "SimpleCamera.h"

// TODO: http://hamelot.co.uk/visualization/moderngl-camera/

SimpleCamera::SimpleCamera( int width, int height, float near, float far, float fov) : 
m_up(0.f,1.f,0.f),m_cameraPos(0.f,0.f,-5.f),m_lookAt(0.f,0.f,0.f)
{
	m_width = width;
	m_height = height;
	
	m_fov = fov;
	m_znear = near;
	m_zfar = far;
}

SimpleCamera::~SimpleCamera(void)
{
}

void SimpleCamera::setWidthHeight(int width, int height) {
	m_width = width;
	m_height = height;
}

void SimpleCamera::update( GLFWwindow* window)
{
	m_viewmatrix = glm::lookAt(m_cameraPos, m_lookAt, m_up);
}

void SimpleCamera::setPosition(const glm::vec3 &position)
{
	m_cameraPos = position;
}

void SimpleCamera::setLookAt(const glm::vec3 &lookAt)
{
	m_lookAt = lookAt;
}

void SimpleCamera::setFOV(float fov)
{
	m_fov = fov;
}

float SimpleCamera::getFOV()
{
	return m_fov;
}

glm::vec3& SimpleCamera::getLookAt()
{
	return m_lookAt;
}

glm::vec3& SimpleCamera::getPosition()
{
	return m_cameraPos;
}

glm::vec3& SimpleCamera::getUp()
{
	return m_up;
}

glm::vec3 SimpleCamera::getDirection()
{
	glm::vec3 dir = glm::normalize(getLookAt() - getPosition());
	return dir;
}

void SimpleCamera::moveBy(const glm::vec3 &delta) {
	setLookAt(getLookAt() + delta);
	setPosition(getPosition() + delta);
}

void SimpleCamera::rotateBy(const float dX, const float dY) {
	glm::vec3 viewDir = glm::normalize(getLookAt() - getPosition());
	glm::vec3 side = glm::cross(viewDir, m_up);

	rotateByQuaternion(dX, glm::vec3(m_up));
	rotateByQuaternion(dY, glm::vec3(side));
}

void SimpleCamera::rotateByQuaternion(const float angle, const glm::vec3 axis) {
	glm::vec3 viewDir = glm::normalize(getLookAt() - getPosition());
	glm::quat quatView(0, viewDir);
	glm::quat quatViewResult = glm::rotate(quatView, angle, axis);
	viewDir.x = quatViewResult.x;
	viewDir.y = quatViewResult.y;
	viewDir.z = quatViewResult.z;
	viewDir = glm::normalize(viewDir);
	setLookAt(viewDir + getPosition());
}

void SimpleCamera::getView( glm::mat4 *view) {
	CVK::Camera::getView(view);
}

void SimpleCamera::getPerspective( glm::mat4 *perspective) {
	CVK::Camera::getPerspective(perspective);
}
