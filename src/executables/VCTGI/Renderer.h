#ifndef __SHADER_RENDERER_H
#define __SHADER_RENDERER_H

#include "Singleton.h"
#include "Context.h"
#include "Profiler.h"

#include "Common.h"
#include <CVK_Framework/CVK_Framework.h>

#include "ShaderCompositing.h"
#include "ShaderGBuffer.h"
#include "ShaderVoxelization.h"
#include "ShaderDirectIllumination.h"
#include "ShaderGlobalIllumination.h"
#include "ShaderPreIntegration.h"
#include "ShaderVoxelViewer.h"

#include "SimpleCamera.h"

enum tw_EScene {
	SCENE_CARS,
	SCENE_CORNELL_BOX,
	SCENE_SPONZA
};

enum tw_ERenderTarget {
	RENDER_POSITION = 0,
	RENDER_NORMAL,
	RENDER_COLOR,
	RENDER_VOXELS,
	RENDER_DIRECT,
	RENDER_GLOBAL,
	RENDER_AO,
	RENDER_COMPOSITION,
	E_NUM_RENDER_TARGETS
};

class Renderer : public Singleton <Renderer>
{
friend class Singleton <Renderer>;
public:
	~Renderer();
	void init();
	void run();
	
	tw_EScene 		  	tw_scene;
	tw_ERenderTarget  	tw_renderTarget;
	unsigned int		tw_mipLevel;
	bool	  		  	tw_updateVoxels;
	bool  			  	tw_wireframe;
	unsigned int 		tw_shadowMapSize;  // TODO
	bool  			  	tw_boundingbox;
	bool  			  	tw_voxelCenters;
	bool  			  	tw_quadraticAttenuation;  // TODO
	bool  			  	tw_percentageCloserFiltering;  // TODO
	float 			  	tw_maximalTracingDistance;
	float  			  	tw_ambientOcclusionAlpha;
	float  			  	tw_ambientOcclusionFalloff;
	float  			  	tw_globalIlluminationAlpha;
	float  			  	tw_specularAlpha;
	float  			  	tw_specularAperture;
	float 			  	tw_glossiness;
	float 			  	tw_shadowMapBias;
	float 			  	tw_speed;
	float 			  	tw_rotationSpeed;
	glm::vec3 		  	tw_lightColor;
	glm::vec3 		  	tw_backgroundColor;
	unsigned int		tw_computeWorkgroupSize;
	
	bool   				m_shadersNeedReloading;
	bool  				m_sceneNeedsReloading;
	bool  				m_texturesNeedReinitialization;
	double 				m_deltaT;
	
	unsigned int 		m_voxelDepth;
	unsigned int 		m_voxelRes;
	unsigned int 		m_voxelAttributes;
	unsigned int 		m_voxelCount;
	float        		m_voxelSize;

	glm::vec3  			m_BBDiagonal;
	float      			m_BBSize;
	float      			m_BBSize2;
	float      			m_BBScale;
	glm::vec3  			m_BBMin;
	glm::vec3  			m_BBCenter;

	CVK::Node* 			m_BBCube;
	CVK::Node* 			m_scene;

	SimpleCamera 		m_camera;
	SimpleCamera 		m_cameraLight;
	
	std::vector<GLuint> m_voxelTextures;

	CVK::ShaderSimpleTexture* 	 m_simpleTextureShader;
	ShaderVoxelViewer* 			 m_voxelViewerShader;
	ShaderGBuffer* 				 m_gBufferShader;
	CVK::ShaderMinimal* 		 m_shadowMapShader;
	ShaderVoxelization* 		 m_voxelizationShader;
	ShaderPreIntegration* 		 m_preIntegrationShader;
	ShaderDirectIllumination* 	 m_directIlluminationShader;
	ShaderGlobalIllumination* 	 m_globalIlluminationShader;
	ShaderCompositing* 			 m_compositingShader;
	CVK::ShaderSimpleTexture* 	 m_FXAAShader;
	
	CVK::FBO* m_shadowMapFBO;
	CVK::FBO* m_gBufferFBO;
	CVK::FBO* m_voxelizationFBO;
	CVK::FBO* m_directIlluminationFBO;
	CVK::FBO* m_globalIlluminationMultisampleFBO;
	CVK::FBO* m_globalIlluminationFBO;
	CVK::FBO* m_compositingFBO;
protected:
	Renderer();	
};

#endif /* __SHADER_RENDERER_H */
