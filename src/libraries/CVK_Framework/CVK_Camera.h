#ifndef __CVK_CAMERA_H
#define __CVK_CAMERA_H

#include "CVK_Defs.h"

namespace CVK
{

class Camera
{
public:
	Camera();
	~Camera();

	virtual void update( GLFWwindow* window) = 0;

	void getView( glm::mat4 *view);
	void getPerspective( glm::mat4 *perspective);
	void getOrtho( glm::mat4 *ortho);
	void useOrtho( float useOrtho);
	void setOrtho( float left, float right, float bottom, float top, float near, float far);
	void getProjection( glm::mat4 *projection);
	void setNearFar( float near, float far);
	void getNearFar( float *near, float *far);
	void setWidthHeight( int width, int height);

protected:
	float m_fov, m_znear, m_zfar;
	int m_width, m_height;
	float m_orthoValues[6];
	bool m_useOrtho;

	glm::mat4 m_viewmatrix;
	glm::mat4 m_perspective;
	glm::mat4 m_ortho;
};

};

#endif /* __CVK_CAMERA_H */
