#include "CVK_State.h"
#include "CVK_ShaderMinimal.h"

CVK::State* CVK::State::g_instance = NULL;

CVK::State::State()
{
	m_camera = 0;
	m_shader = 0;
	m_cubeMap = 0;
}

CVK::State::~State()
{
}

CVK::State* CVK::State::getInstance()
{
	if( g_instance == NULL)
	{	
		g_instance = new CVK::State();
	}
	return g_instance;
}

void CVK::State::setShader( CVK::ShaderMinimal* shader)
{
	m_shader = shader;
	shader->useProgram();
}

CVK::ShaderMinimal* CVK::State::getShader()
{
	return m_shader;
}

void CVK::State::setCubeMapTexture( CVK::CubeMapTexture* cubeMap)
{
	m_cubeMap = cubeMap;
}

CVK::CubeMapTexture* CVK::State::getCubeMapTexture()
{
	return m_cubeMap;
}

void CVK::State::addLight( CVK::Light* light)
{
	m_lights.push_back(light);
}

void CVK::State::setLight( int index, CVK::Light* light)
{
	if (index >= 0 && index < m_lights.size())
		m_lights[index] = light;
}

void CVK::State::removeLight( int indexToRemove)
{
	if (indexToRemove >= 0 && indexToRemove < m_lights.size())
		m_lights.erase(m_lights.begin() + indexToRemove);
}

std::vector<CVK::Light*>* CVK::State::getLights()
{
	return &m_lights;
}

void CVK::State::setCamera( CVK::Camera* camera)
{
	m_camera = camera;
}

CVK::Camera* CVK::State::getCamera( )
{
	return m_camera;
}

void CVK::State::updateSceneSettings( glm::vec3 lightAmbient, int fogMode, glm::vec3 fogCol, float fogStart, float fogEnd, float fogDens)
{
	m_lightAmbient = lightAmbient;
	m_fogMode = fogMode;
	m_fogCol = fogCol;
	m_fogStart = fogStart;
	m_fogEnd = fogEnd;
	m_fogDens = fogDens;
}

glm::vec3 CVK::State::getLightAmbient()
{
	return m_lightAmbient;
}

int CVK::State::getFogMode()
{
	return m_fogMode;
}

glm::vec3 CVK::State::getFogCol()
{
	return m_fogCol;
}

float CVK::State::getFogStart()
{
	return m_fogStart;
}

float CVK::State::getFogEnd()
{
	return m_fogEnd;
}

float CVK::State::getFogDens()
{
	return m_fogDens;
}




