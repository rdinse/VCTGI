#ifndef __SIMPLECAMERA_H
#define __SIMPLECAMERA_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>


class SimpleCamera : public CVK::Camera
{
	public:
		SimpleCamera( int width, int height, float near = 0.1f, float far = 10.f, float fov = 60.f);
		~SimpleCamera(void);
	
		virtual void update( GLFWwindow* window);

		void setPosition(glm::vec3 &position);
		glm::vec3& getPosition();

		void setLookAt(glm::vec3 &lookAt);
		glm::vec3& getLookAt();

		void setFOV(float fov);
		float getFov();

	private:
		glm::vec3 m_cameraPos, m_lookAt, m_up;
};

#endif

