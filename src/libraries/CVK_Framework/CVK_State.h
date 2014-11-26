#ifndef __CVK_STATE_H
#define __CVK_STATE_H

#include "CVK_Defs.h"
#include "CVK_Light.h"
#include "CVK_Camera.h"
#include "CVK_CubeMapTexture.h"

namespace CVK
{

// forward declaration
class ShaderMinimal;

class State 
{
public:
	static State* getInstance();

	void setShader( CVK::ShaderMinimal* shader);
	CVK::ShaderMinimal* getShader();

	void setCubeMapTexture( CVK::CubeMapTexture* cubeMap);
	CVK::CubeMapTexture* getCubeMapTexture();

	void addLight( CVK::Light* light);
	void setLight( int index, CVK::Light* light);
	void removeLight( int indexToRemove);
	std::vector<CVK::Light*>* getLights( );

	void setCamera( CVK::Camera* camera);
	CVK::Camera* getCamera( );

	void updateSceneSettings( glm::vec3 lightAmbient, int fogMode, glm::vec3 fogCol, float fogStart, float fogEnd, float fogDens);
	glm::vec3 getLightAmbient();
	int getFogMode();
	glm::vec3 getFogCol();
	float getFogStart();
	float getFogEnd();
	float getFogDens();
	
private:
	State();
	~State();

	CVK::ShaderMinimal* m_shader;

	CVK::CubeMapTexture* m_cubeMap;

	std::vector<CVK::Light*> m_lights;

	CVK::Camera* m_camera;

	glm::vec3 m_lightAmbient;
	int m_fogMode;
	glm::vec3 m_fogCol;
	float m_fogStart;
	float m_fogEnd;
	float m_fogDens;
	
	static State* g_instance;
};

};

#endif /* __CVK_STATE_H */

