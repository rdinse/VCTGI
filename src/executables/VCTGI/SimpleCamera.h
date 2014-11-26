#ifndef __SIMPLECAMERA_H
#define __SIMPLECAMERA_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>


class SimpleCamera : public CVK::Camera {
	public:
		SimpleCamera( int width, int height, float near = 0.1f, float far = 10.f, float fov = 60.f);
		~SimpleCamera(void);

		virtual void update( GLFWwindow* window);
		
		void setWidthHeight(int width, int height);
		
		void setPosition(const glm::vec3 &position);
		glm::vec3& getPosition();

		void setLookAt(const glm::vec3 &lookAt);
		glm::vec3& getLookAt();

		glm::vec3& getUp();
		glm::vec3 getDirection();

		void moveBy(const glm::vec3 &delta);
		void rotateBy(const float dX, const float dY);
		void rotateByQuaternion(const float angle, const glm::vec3 axis);

		void setFOV(float fov);
		float getFOV();

		void getView( glm::mat4 *view);
		void getPerspective( glm::mat4 *perspective);

	private:
		glm::vec3 m_cameraPos, m_lookAt, m_up;
};

#endif

