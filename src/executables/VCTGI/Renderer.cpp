#include "Renderer.h"

std::map<tw_EScene, const char *> gSceneFiles = {
	{SCENE_CARS, "/cars/obj/cars1.obj"},
	{SCENE_CORNELL_BOX, "/cornellbox/CornellBox.obj"},
	{SCENE_SPONZA, "/crytek-sponza/sponza.obj"}
};
std::map<tw_EScene, glm::mat4> gSceneModelMatrices = {
	{SCENE_CARS, glm::scale(glm::rotate(glm::mat4(1.0f), 180.0f, glm::vec3(0, 1, 0)), glm::vec3(0.95 * 0.5))},
	{SCENE_CORNELL_BOX, glm::mat4(1.0f)},
	{SCENE_SPONZA, glm::scale(glm::mat4(1.0f), glm::vec3(0.95f * 1.0f / 1500.0f))}
};
std::map<tw_EScene, glm::vec3> gSceneAABBMin = {
	{SCENE_CARS, glm::vec3(-1.0f, -1.0f, -1.0f)},
	{SCENE_CORNELL_BOX, glm::vec3(-1.0f, -1.0f, -1.0f)},
	{SCENE_SPONZA, glm::vec3(-1.0f, -1.0f, -1.0f)}
};
std::map<tw_EScene, glm::vec3> gSceneAABBMax = {
	{SCENE_CARS, glm::vec3(1.0f, 1.0f, 1.0f)},
	{SCENE_CORNELL_BOX, glm::vec3(1.0f, 1.0f, 1.0f)},
	{SCENE_SPONZA, glm::vec3(1.0f, 1.0f, 1.0f)}
};
std::map<tw_EScene, glm::vec3> gSceneLightPosition = {
	{SCENE_CARS, 1.2f * glm::vec3(-2.0f, 2.0f, 3.0f)},
	{SCENE_CORNELL_BOX, 0.8f * glm::vec3(-2.0f, 2.0f, 3.0f)},
	{SCENE_SPONZA, glm::vec3(0.2f, 2.0f, 0.0f)}
};

Renderer::Renderer() :
	m_camera(0, 0),
	m_cameraLight(0, 0)
{
	tw_scene = SCENE_CARS;

	tw_renderTarget = RENDER_COMPOSITION;
	tw_mipLevel = 0;
	tw_updateVoxels = true;
	tw_wireframe = false;
	tw_boundingbox = false;
	tw_voxelCenters = false;
	tw_quadraticAttenuation = false;  // TODO
	tw_percentageCloserFiltering = false;  // TODO
	tw_maximalTracingDistance = glm::sqrt(3.0f);
	tw_ambientOcclusionFalloff = 725.0f;
	tw_ambientOcclusionAlpha = 0.55f;
	tw_globalIlluminationAlpha = 0.75f;
	tw_specularAlpha = 1.00;
	tw_specularAperture = 0.158;
	tw_glossiness = 1.0f;  // TODO
	tw_shadowMapBias = 0.000085f;
	tw_speed = 1.5f;
	tw_rotationSpeed = 0.3f;
	tw_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	tw_backgroundColor = glm::vec3(0.0f, 0.0f, 0.05f);

	m_deltaT = 0;
	m_shadersNeedReloading = false;
	m_sceneNeedsReloading = false;
	m_texturesNeedReinitialization = false;

	m_BBDiagonal = gSceneAABBMax[tw_scene] - gSceneAABBMin[tw_scene];
	m_BBSize = glm::max(m_BBDiagonal.x, glm::max(m_BBDiagonal.y, m_BBDiagonal.z));
	m_BBSize2 = m_BBSize / 2.0f;
	m_BBScale = 1.0f / m_BBSize;
	m_BBMin = gSceneAABBMin[tw_scene];
	m_BBCenter = m_BBMin + glm::vec3(m_BBSize2);

	m_BBCube = new CVK::Node("Bounding Box");
	CVK::Cube* cube = new CVK::Cube(m_BBSize2);
	m_BBCube->setGeometry(cube);
	m_BBCube->setMaterial(new CVK::Material(glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::mat4 boundingBoxCubeModelMatrix = glm::translate(m_BBCenter);
	m_BBCube->setModelMatrix(boundingBoxCubeModelMatrix);

	m_voxelDepth = 8;
	m_voxelRes = 1 << m_voxelDepth;
	m_voxelCount = m_voxelRes * m_voxelRes * m_voxelRes;
	m_voxelSize = m_BBSize / m_voxelRes;

	m_voxelAttributes = 6;

	for (int i = 0; i < m_voxelAttributes; i++) {
		m_voxelTextures.push_back(INVALID_OGL_VALUE);
	}

	tw_shadowMapSize = 2048;

	tw_computeWorkgroupSize = 4;

	m_simpleTextureShader = NULL;
	m_voxelViewerShader = NULL;
	m_gBufferShader = NULL;
	m_shadowMapShader = NULL;
	m_voxelizationShader = NULL;
	m_directIlluminationShader = NULL;
	m_globalIlluminationShader = NULL;
	m_compositingShader = NULL;
	m_shadowMapFBO = NULL;
	m_gBufferFBO = NULL;
	m_voxelizationFBO = NULL;
	m_directIlluminationFBO = NULL;
	m_globalIlluminationFBO = NULL;
	m_scene = NULL;
}

Renderer::~Renderer() {
	glDeleteTextures(m_voxelAttributes, &m_voxelTextures[0]);
	delete Context::getInstance();
	delete Profiler::getInstance();
}

void Renderer::init() {
	Context::getInstance()->init();

	// Camera setup
	m_camera.setWidthHeight(Context::getInstance()->getWidth(), Context::getInstance()->getHeight());
	m_camera.setLookAt(glm::vec3(-0.45f, 0.0f, 0.0f));
	m_camera.setPosition(glm::vec3(-0.45f, 0.0f, 3.0f));
	m_camera.setNearFar(0.1f, 100.0f);
	CVK::State::getInstance()->setCamera(&m_camera);

	m_cameraLight.setWidthHeight(tw_shadowMapSize, tw_shadowMapSize);
	m_cameraLight.setPosition(gSceneLightPosition[tw_scene]);
	m_cameraLight.setNearFar(0.1f, 100.0f);
	m_cameraLight.setFOV(glm::degrees(45));

	// Scene graph setup
	m_scene = new CVK::Node("Scene", std::string(RESOURCES_PATH) + gSceneFiles[tw_scene]);
	m_scene->setModelMatrix(gSceneModelMatrices[tw_scene]);

	const char* m_shadernamesVoxelViewersimpleTexture[] = {SHADERS_PATH "/VCTGI/ScreenFill.vert", SHADERS_PATH "/VCTGI/ScreenFillSimpleTexture.frag"};
	m_simpleTextureShader = new CVK::ShaderSimpleTexture(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesVoxelViewersimpleTexture);

	const char* m_shadernamesVoxelViewer[] = {SHADERS_PATH "/VCTGI/VoxelViewer.vert", SHADERS_PATH "/VCTGI/VoxelViewer.geom", SHADERS_PATH "/VCTGI/VoxelViewer.frag"};
	m_voxelViewerShader = new ShaderVoxelViewer(VERTEX_SHADER_BIT|GEOMETRY_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesVoxelViewer);

	m_shadowMapFBO = new CVK::FBO(tw_shadowMapSize, tw_shadowMapSize, 1, true);
	const char* m_shadernamesShadowMap[] = {SHADERS_PATH "/VCTGI/ShadowMap.vert", SHADERS_PATH "/VCTGI/ShadowMap.frag"};
	m_shadowMapShader = new CVK::ShaderMinimal(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesShadowMap);

	// 4 attachments: Position, Normal, Color, Tangent
	m_gBufferFBO = new CVK::FBO(Context::getInstance()->getWidth(), Context::getInstance()->getHeight(), 4, true);
	const char *m_shadernamesGBuffer[] = {SHADERS_PATH "/VCTGI/GBuffer.vert", SHADERS_PATH "/VCTGI/GBuffer.frag"};
	m_gBufferShader = new ShaderGBuffer(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesGBuffer);

	// Zero attachments
	m_voxelizationFBO = new CVK::FBO(m_voxelRes, m_voxelRes, m_voxelAttributes, false, false, true);
	const char *m_shadernamesVoxelization[] = {SHADERS_PATH "/VCTGI/Voxelization.vert", SHADERS_PATH "/VCTGI/Voxelization.geom", SHADERS_PATH "/VCTGI/Voxelization.frag"};
	m_voxelizationShader = new ShaderVoxelization(VERTEX_SHADER_BIT|GEOMETRY_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesVoxelization);

	const char *m_shadernamesPreIntegration[] = {SHADERS_PATH "/VCTGI/PreIntegration.comp"};
	m_preIntegrationShader = new ShaderPreIntegration(COMPUTE_SHADER_BIT, m_shadernamesPreIntegration);

	m_directIlluminationFBO = new CVK::FBO(Context::getInstance()->getWidth(), Context::getInstance()->getHeight(), 1);
	const char *m_shadernamesDirectIllumination[] = {SHADERS_PATH "/VCTGI/ScreenFill.vert", SHADERS_PATH "/VCTGI/ScreenFillDirectIllumination.frag"};
	m_directIlluminationShader = new ShaderDirectIllumination(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesDirectIllumination);

	// 2 attachments: Global Illumination, Ambient Occlusion
	m_globalIlluminationFBO = new CVK::FBO(Context::getInstance()->getWidth() / 4, Context::getInstance()->getHeight() / 4, 2, false, false, false, 4);
	const char *m_shadernamesGlobalIllumination[] = {SHADERS_PATH "/VCTGI/ScreenFill.vert", SHADERS_PATH "/VCTGI/ScreenFillGlobalIllumination.frag"};
	m_globalIlluminationShader = new ShaderGlobalIllumination(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesGlobalIllumination);

	m_compositingFBO = new CVK::FBO(Context::getInstance()->getWidth(), Context::getInstance()->getHeight(), 1);
	const char *m_shadernamesCompositing[] = {SHADERS_PATH "/VCTGI/ScreenFill.vert", SHADERS_PATH "/VCTGI/ScreenFillCompositing.frag"};
	m_compositingShader = new ShaderCompositing(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesCompositing);

	const char *m_shadernamesFXAA[] = {SHADERS_PATH "/VCTGI/ScreenFill.vert", SHADERS_PATH "/VCTGI/ScreenFillFXAA.frag"};
	m_FXAAShader = new CVK::ShaderSimpleTexture(VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, m_shadernamesFXAA);

	m_simpleTextureShader->getUniformLocations();
	m_voxelViewerShader->getUniformLocations();
	m_gBufferShader->getUniformLocations();
	m_shadowMapShader->getUniformLocations();
	m_voxelizationShader->getUniformLocations();
	m_preIntegrationShader->getUniformLocations();
	m_directIlluminationShader->getUniformLocations();
	m_globalIlluminationShader->getUniformLocations();
	m_compositingShader->getUniformLocations();

	m_voxelizationShader->setTextureInput(2, m_shadowMapFBO->getDepthTexture());           // Shadow Map
	m_directIlluminationShader->setTextureInput(0, m_gBufferFBO->getColorTexture(0));      // Position
	m_directIlluminationShader->setTextureInput(1, m_gBufferFBO->getColorTexture(1));      // Normals
	m_directIlluminationShader->setTextureInput(2, m_shadowMapFBO->getDepthTexture());     // Shadow Map
	m_globalIlluminationShader->setTextureInput(0, m_gBufferFBO->getColorTexture(0));      // Position
	m_globalIlluminationShader->setTextureInput(1, m_gBufferFBO->getColorTexture(1));      // Normals
	m_globalIlluminationShader->setTextureInput(2, m_gBufferFBO->getColorTexture(3));      // Tangents
	m_compositingShader->setTextureInput(0, m_gBufferFBO->getColorTexture(2));             // Color
	m_compositingShader->setTextureInput(1, m_gBufferFBO->getColorTexture(1));             // Normal
	m_compositingShader->setTextureInput(2, m_gBufferFBO->getDepthTexture());              // Depth
	m_compositingShader->setTextureInput(3, m_directIlluminationFBO->getColorTexture(0));  // Direct illumination
	m_compositingShader->setTextureInput(4, m_globalIlluminationFBO->getColorTexture(0));  // Global illumination
	m_compositingShader->setTextureInput(5, m_globalIlluminationFBO->getColorTexture(1));  // Ambient occlusion
	m_FXAAShader->setTextureInput(0, m_compositingFBO->getColorTexture(0));				         // Composed image
	m_FXAAShader->setTextureInput(1, m_gBufferFBO->getDepthTexture());				             // Depth
	m_FXAAShader->setTextureInput(2, m_gBufferFBO->getColorTexture(1));     			         // Normals
}

void Renderer::run() {
	Profiler::getInstance()->updateProfilerFirst(PASS_INIT);

	glPointSize(2.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	float lightT = glm::pow(glm::min(glm::abs(m_cameraLight.getPosition().x) / 3.5f, 1.0f), 3.5f);
	Renderer::getInstance()->tw_lightColor = (1 - lightT) * glm::vec3(0.752941f, 0.749020f, 0.678431f)
												 + lightT * glm::vec3(0.713725f, 0.494118f, 0.356863f);

	bool isFirstFrame = true;
	double lastTime = glfwGetTime();
	while(!glfwWindowShouldClose(Context::getInstance()->getWindow()))
	{
		// Update delta time and FPS.
		double currentTime = glfwGetTime();
		m_deltaT = currentTime - lastTime;
		lastTime = currentTime;
		Profiler::getInstance()->updateFPS(m_deltaT);
		glEnable(GL_DEPTH_TEST);

		Profiler::getInstance()->updateProfiler(PASS_G_BUFFER, isFirstFrame);
		{
			m_gBufferFBO->bind();
			//glEnable(GL_CULL_FACE);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			CVK::State::getInstance()->setCamera(&m_camera);
			m_camera.update(Context::getInstance()->getWindow());
			CVK::State::getInstance()->setShader(m_gBufferShader);
			m_gBufferShader->update();

			m_scene->render();

			if (tw_boundingbox) {
				glDisable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				m_BBCube->render();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_CULL_FACE);
			}

			if (tw_voxelCenters) {
				m_gBufferShader->resetModelMatrix();
				glBindVertexArray(m_voxelViewerShader->m_voxelGridPointsVAO);
				glDrawArrays(GL_POINTS, 0, Renderer::getInstance()->m_voxelCount);
				glBindVertexArray(0);
			}

			m_gBufferFBO->unbind();
		}

		Profiler::getInstance()->updateProfiler(PASS_SHADOW_MAP, isFirstFrame);
		{
			m_shadowMapFBO->bind();
			glDisable(GL_CULL_FACE);
			glClear(GL_DEPTH_BUFFER_BIT);
			CVK::State::getInstance()->setCamera(&m_cameraLight);
			m_cameraLight.update(Context::getInstance()->getWindow());
			CVK::State::getInstance()->setShader(m_shadowMapShader);
			m_shadowMapShader->update();
			m_scene->render();
			m_shadowMapFBO->unbind();

		}

		if (tw_updateVoxels) {
			Profiler::getInstance()->updateProfiler(PASS_VOXELIZATION_CLEAR, isFirstFrame);
			{
				const GLfloat clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

				for (int i = 0; i < m_voxelAttributes; i++) {
					glClearTexImage(m_voxelTextures[i], 0, GL_RGBA, GL_FLOAT, clearColor);
				}
			}

			Profiler::getInstance()->updateProfiler(PASS_VOXELIZATION, isFirstFrame);
			{
				m_voxelizationFBO->bind();
				glDisable(GL_DEPTH_TEST);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				CVK::State::getInstance()->setShader(m_voxelizationShader);
				m_voxelizationShader->update();
				m_scene->render();
				glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				m_voxelizationFBO->unbind();
			}

			Profiler::getInstance()->updateProfiler(PASS_PRE_INTEGRATION, isFirstFrame);
			{
				m_preIntegrationShader->useProgram();
				m_preIntegrationShader->run();
			}
		}

		Profiler::getInstance()->updateProfiler(PASS_DIRECT_ILLUMINATION, isFirstFrame);
		{
			m_directIlluminationFBO->bind();
			glClear(GL_COLOR_BUFFER_BIT
					| GL_DEPTH_BUFFER_BIT);
			m_directIlluminationShader->useProgram();
			m_directIlluminationShader->update();
			m_directIlluminationShader->render();
			m_directIlluminationFBO->unbind();
		}

		Profiler::getInstance()->updateProfiler(PASS_GLOBAL_ILLUMINATION, isFirstFrame);
		{
		    // 0.375 0.125
		    // 0.875 0.375
		    // 0.125 0.625
		    // 0.625 0.875
			m_globalIlluminationFBO->bind();
			glEnable(GL_SAMPLE_SHADING_ARB);
		    glEnable(GL_MULTISAMPLE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_globalIlluminationShader->useProgram();
			m_globalIlluminationShader->update();
			m_globalIlluminationShader->render();
		    glDisable(GL_MULTISAMPLE);
			glDisable(GL_SAMPLE_SHADING_ARB);
			m_globalIlluminationFBO->unbind();
		}

		Profiler::getInstance()->updateProfiler(PASS_COMPOSITING, isFirstFrame);
		{
			m_compositingFBO->bind();
			m_compositingShader->useProgram();
			m_compositingShader->setOutputMode(0);
			m_compositingShader->update();
			m_compositingShader->render();
			m_compositingFBO->unbind();
		}

		Profiler::getInstance()->updateProfiler(PASS_DRAW, isFirstFrame);
		{
			glViewport(0, 0, Context::getInstance()->getWidth(), Context::getInstance()->getHeight());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_BLEND);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if(tw_renderTarget < 3) {
				m_simpleTextureShader->useProgram();
				m_simpleTextureShader->setTextureInput(0, m_gBufferFBO->getColorTexture(tw_renderTarget));
				m_simpleTextureShader->update();
				m_simpleTextureShader->render();

			} else if(tw_renderTarget == RENDER_VOXELS) {
				glEnable(GL_DEPTH_TEST);
				m_voxelViewerShader->useProgram();
				m_voxelViewerShader->update();
				m_voxelViewerShader->render();

			} else if(tw_renderTarget == RENDER_DIRECT) {
				m_simpleTextureShader->useProgram();
				m_simpleTextureShader->setTextureInput(0, m_directIlluminationFBO->getColorTexture(0));
				m_simpleTextureShader->update();
				m_simpleTextureShader->render();

			} else if(tw_renderTarget == RENDER_GLOBAL) {
				m_compositingShader->useProgram();
				m_compositingShader->setOutputMode(1);
				m_compositingShader->update();
				m_compositingShader->render();

			} else if(tw_renderTarget == RENDER_AO) {
				m_compositingShader->useProgram();
				m_compositingShader->setOutputMode(2);
				m_compositingShader->update();
				m_compositingShader->render();

			} else if(tw_renderTarget == RENDER_COMPOSITION) {
				m_FXAAShader->useProgram();
				m_FXAAShader->update();
				m_FXAAShader->render();
			}
		}

		Profiler::getInstance()->updateProfiler(PASS_INIT, isFirstFrame);
		Context::getInstance()->afterDrawUpdate();

		if (m_shadersNeedReloading) {
			bool noError = true;

			noError &= m_FXAAShader->reloadShaders();
			noError &= m_globalIlluminationShader->reloadShaders();
			noError &= m_directIlluminationShader->reloadShaders();
			noError &= m_voxelizationShader->reloadShaders();
			noError &= m_preIntegrationShader->reloadShaders();
			noError &= m_gBufferShader->reloadShaders();
			noError &= m_shadowMapShader->reloadShaders();
			noError &= m_voxelViewerShader->reloadShaders();
			noError &= m_simpleTextureShader->reloadShaders();
			noError &= m_compositingShader->reloadShaders();
			noError &= m_FXAAShader->reloadShaders();

			if (!noError) {
				std::cout << "Error: Shaders could not be reloaded." << std::endl;
			} else {
				m_simpleTextureShader->getUniformLocations();
				m_voxelViewerShader->getUniformLocations();
				m_gBufferShader->getUniformLocations();
				m_shadowMapShader->getUniformLocations();
				m_voxelizationShader->getUniformLocations();
				m_preIntegrationShader->getUniformLocations();
				m_directIlluminationShader->getUniformLocations();
				m_globalIlluminationShader->getUniformLocations();
				m_compositingShader->getUniformLocations();
				m_FXAAShader->getUniformLocations();

				std::cout << "Shaders reloaded." << std::endl;
			}

			m_shadersNeedReloading = false;
		}

		if (m_texturesNeedReinitialization) {
			m_voxelRes = 1 << m_voxelDepth;
			m_voxelCount = m_voxelRes * m_voxelRes * m_voxelRes;
			m_voxelSize = m_BBSize / m_voxelRes;
			tw_mipLevel = glm::min(tw_mipLevel, m_voxelDepth);
			m_voxelizationFBO->resize(m_voxelRes, m_voxelRes);

			m_voxelViewerShader->prepareVoxelGrid();
			m_voxelizationShader->prepareTextures();

			m_texturesNeedReinitialization = false;
		}

		if (m_sceneNeedsReloading) {
			// TODO
			m_sceneNeedsReloading = false;
		}

		if (isFirstFrame) {
			isFirstFrame = false;
		}
	}
}
