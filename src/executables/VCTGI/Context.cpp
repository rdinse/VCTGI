#include "Context.h"
#include "Renderer.h"
#include "Profiler.h"

extern std::map<tw_EProfilerPoint, const char *> tw_profilerPointNames;

//======================================================================================
// GLFW and AntTweakBar Callback Functions
//======================================================================================

void resizeCallback(GLFWwindow *window, int w, int h) {
	Context::getInstance()->setSize(w, h);
	Renderer::getInstance()->m_camera.setWidthHeight(w, h);
	TwWindowSize(w, h);
}

double gMousePosX, gMousePosY, gMousePressPosX, gMousePressPosY;
glm::vec3 gSpeed = glm::vec3(0);
bool gMousePressed;
void pollMouseAndKeyboardEvents(GLFWwindow* window) {

	SimpleCamera& camera = Renderer::getInstance()->m_camera;
	SimpleCamera& cameraLight = Renderer::getInstance()->m_cameraLight;

	glm::vec3 cameraUp = camera.getUp();
	glm::vec3 cameraPos = camera.getPosition();
	glm::vec3 cameraDir = camera.getDirection();
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDir, cameraUp));
	glm::vec3 cameraLightUp = cameraLight.getUp();
	glm::vec3 cameraLightPos = cameraLight.getPosition();
	glm::vec3 cameraLightDir = cameraLight.getDirection();
	glm::vec3 cameraLightRight = glm::normalize(glm::cross(cameraLightDir, cameraLightUp));
	
	bool lightUpdateNeeded = false;

	float threshold = 2.0f;
	float acceleration = 9.0f;
	float deacceleration = 4.0f;
	double deltaT = Renderer::getInstance()->m_deltaT;
	float speed = Renderer::getInstance()->tw_speed;

	gSpeed = glm::clamp(gSpeed, -glm::vec3(Renderer::getInstance()->tw_speed), glm::vec3(Renderer::getInstance()->tw_speed));
	camera.moveBy(-gSpeed * (float)deltaT);

	if (glm::abs(gSpeed.x) >= threshold * (float)deltaT) gSpeed.x -= glm::normalize(gSpeed.x) * (float)deltaT * deacceleration; else gSpeed.x = 0.0f;
	if (glm::abs(gSpeed.y) >= threshold * (float)deltaT) gSpeed.y -= glm::normalize(gSpeed.y) * (float)deltaT * deacceleration; else gSpeed.y = 0.0f;
	if (glm::abs(gSpeed.z) >= threshold * (float)deltaT) gSpeed.z -= glm::normalize(gSpeed.z) * (float)deltaT * deacceleration; else gSpeed.z = 0.0f;
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		gSpeed -= -acceleration * (float)deltaT * cameraRight;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		gSpeed -=  acceleration * (float)deltaT * cameraDir;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		gSpeed -= -acceleration * (float)deltaT * cameraDir;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		gSpeed -=  acceleration * (float)deltaT * cameraRight;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		gSpeed -= -acceleration * (float)deltaT * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		gSpeed -=  acceleration * (float)deltaT * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		cameraLight.setPosition(cameraLight.getPosition() + ((-0.7f * speed * (float)deltaT) * cameraLightRight));
		cameraLight.moveBy(glm::vec3(0, 0.00001 * cameraLight.getPosition().x, 0));
		lightUpdateNeeded = true;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		cameraLight.setPosition(cameraLight.getPosition() + (( 0.7f * speed * (float)deltaT) * cameraLightUp));
		lightUpdateNeeded = true;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		cameraLight.setPosition(cameraLight.getPosition() + ((-0.7f * speed * (float)deltaT) * cameraLightUp));
		lightUpdateNeeded = true;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		cameraLight.setPosition(cameraLight.getPosition() + (( 0.7f * speed * (float)deltaT) * cameraLightRight));
		cameraLight.moveBy(glm::vec3(0, -0.002 * cameraLight.getPosition().x, 0));
		lightUpdateNeeded = true;
	}

	if (lightUpdateNeeded) {
		float lightT = glm::pow(glm::min(glm::abs(cameraLight.getPosition().x) / 3.5f, 1.0f), 3.5f);
		Renderer::getInstance()->tw_lightColor = (1 - lightT) * glm::vec3(0.752941f, 0.749020f, 0.678431f)
													 + lightT * glm::vec3(0.713725f, 0.494118f, 0.356863f);
	}

	// Mouse Control
	double mousePosX, mousePosY;
	glfwGetCursorPos(window, &mousePosX, &mousePosY);

	// Viewing Direction
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (!gMousePressed) {
			gMousePressed = true;
			gMousePressPosX = mousePosX;
			gMousePressPosX = mousePosY;
		}

		float dX = Renderer::getInstance()->tw_rotationSpeed * (mousePosX - gMousePosX);
		float dY = Renderer::getInstance()->tw_rotationSpeed * (mousePosY - gMousePosY);
		camera.rotateBy(dX, dY);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		gMousePressPosX = mousePosX;
		gMousePressPosX = mousePosY;
		gMousePressed = false;
	}

	gMousePosX = mousePosX;
	gMousePosY = mousePosY;
}

inline void TwEventMouseButtonGLFW3 (GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }

inline void TwEventMousePosGLFW3 (GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
		TwMouseMotion(int(xpos), int(ypos));
	}
}

inline void TwEventMouseWheelGLFW3 (GLFWwindow* window, double xoffset, double yoffset) { TwEventMouseWheelGLFW(yoffset); }

inline void TwEventCharGLFW3 (GLFWwindow* window, int codepoint) {TwEventCharGLFW(codepoint, GLFW_PRESS); }

inline void TwEventKeyGLFW3 (GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	TwEventKeyGLFW(key, action);
}

static void TW_CALL voxelDepthSetCallback(const void *value, void *clientData) {
	*static_cast<int *>(clientData) = *static_cast<const int *>(value);
	Renderer::getInstance()->m_texturesNeedReinitialization = true;
}

static void TW_CALL voxelDepthGetCallback(void *value, void *clientData) {
	*static_cast<int *>(value) = *static_cast<int *>(clientData);
}

void TW_CALL reloadShadersButtonCallback(void* clientData) {
	Renderer::getInstance()->m_shadersNeedReloading = true;
}

//======================================================================================
// End of GLFW and AntTweakBar Callback Functions
//======================================================================================


Context::Context() {
	m_width = DEFAULT_CONTEXT_WIDTH;
	m_height = DEFAULT_CONTEXT_HEIGHT;

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_window = glfwCreateWindow(m_width, m_height, "Voxel Cone Tracing", 0, 0);
	glfwSetWindowPos(m_window, 100, 50);
	glfwMakeContextCurrent(m_window);
	glfwSetWindowSizeCallback(m_window, resizeCallback);

	glewInit();

	printOpenGLInfo();
}

Context::~Context() {	
	TwTerminate();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Context::init() {
	initAntTweakBarAndProfiler();
}

void Context::setSize(int w, int h) {
	m_width = w;
	m_height = w;
}

TwBar* Context::getAntTweakBar() {
	return tw_tweakBar;
}

int Context::getWidth() {
	return m_width;
}

int Context::getHeight() {
	return m_height;
}

GLFWwindow* Context::getWindow() {
	return m_window;
}

void Context::initAntTweakBarAndProfiler() {
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(m_width, m_height);

	tw_tweakBar = TwNewBar("TweakParameterBar");

	TwDefine("TweakParameterBar label='Tweak Parameters' contained='true' fontsize=1 "
			 "color='150 150 150' alpha='50' text='light' position='33 50' size='210 500'");

	glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(m_window, (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(m_window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(m_window, (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(m_window, (GLFWcharfun)TwEventCharGLFW3);

	TwType renderTargetType = TwDefineEnum("renderTargetType", NULL, 0);

	TwAddVarRW(tw_tweakBar, "renderTargetChoice", renderTargetType, &Renderer::getInstance()->tw_renderTarget,
		" label='Rendering Target' group='General' keyIncr='<' keyDecr='>' enum='"
			"0 {g-Buffer Position}, "
			"1 {g-Buffer Normal}, "
			"2 {g-Buffer Color}, "
			"3 {Voxels}, "
			"4 {Direct Illumination}, "
			"5 {Indirect Illumination}, "
			"6 {Ambient Occlusion}, "
			"7 {Final Composition}'");

    TwAddVarCB(tw_tweakBar, "voxelDepth", TW_TYPE_INT32, voxelDepthSetCallback, voxelDepthGetCallback, &Renderer::getInstance()->m_voxelDepth,
    		"step='1' max='8' min='2' label='Mip Levels' group='General'");
	TwAddVarRW(tw_tweakBar, "mipLevel", TW_TYPE_INT32, &Renderer::getInstance()->tw_mipLevel,
			"step='1' max='8' min='0' label='Show Mip Level' group='General'");
	TwAddVarRW(tw_tweakBar, "updateVoxels", TW_TYPE_BOOLCPP, &Renderer::getInstance()->tw_updateVoxels,
			"label='Update Voxels' group='General'");
	TwAddVarRW(tw_tweakBar, "wireframe", TW_TYPE_BOOLCPP, &Renderer::getInstance()->tw_wireframe,
			"label='Draw Wireframe' group='General'");
	TwAddVarRW(tw_tweakBar, "boundingbox", TW_TYPE_BOOLCPP, &Renderer::getInstance()->tw_boundingbox,
			"label='Draw Bounding Box' group='General'");
	TwAddVarRW(tw_tweakBar, "voxelCenters", TW_TYPE_BOOLCPP, &Renderer::getInstance()->tw_voxelCenters,
			"label='Draw Voxel Centers' group='General'");
	TwAddVarRW(tw_tweakBar, "specularAlpha", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_specularAlpha,
			"step='0.01' max='2.0' min='0.0' label='Specular Cone' group='General'");
	TwAddVarRW(tw_tweakBar, "specularAperture", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_specularAperture,
			"step='0.001' max='1.0' min='0.000' label='Shininess (Cone Radius)' group='General'");
	TwAddVarRW(tw_tweakBar, "globalIlluminationAlpha", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_globalIlluminationAlpha,
			"step='0.01' max='2.0' min='0.0' label='Global Illumination' group='General'");
	TwAddVarRW(tw_tweakBar, "ambientOcclusionAlpha", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_ambientOcclusionAlpha,
			"step='0.01' max='1.0' min='0.0' label='Ambient Occlusion' group='General'");
	TwAddVarRW(tw_tweakBar, "ambientOcclusionFalloff", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_ambientOcclusionFalloff,
			"step='1.0' max='10000.0' min='0.0001' label='AO Falloff' group='General'");
	TwAddVarRW(tw_tweakBar, "maximalTracingDistance", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_maximalTracingDistance,
			"step='0.01' max='2.0' min='0.01' label='Maximal Tracing Distance' group='General'");
	TwAddButton(tw_tweakBar, "reloadShaders", reloadShadersButtonCallback, NULL, "label='Reload Shaders' group='General' key='r'");

	TwAddVarRW(tw_tweakBar, "lightColor", TW_TYPE_COLOR3F, &Renderer::getInstance()->tw_lightColor,
			"label='Light Color' group='Light' colormode='hls'");
	TwAddVarRW(tw_tweakBar, "shadowMapBias", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_shadowMapBias,
			"step='0.000001' max='1.0' min='0.0' label='Shadow Map Bias' group='Light'");

	TwAddVarRW(tw_tweakBar, "speed", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_speed,
			"step='0.01' max='5.0' min='0.02' label='Speed' group='Camera'");
	TwAddVarRW(tw_tweakBar, "rotationSpeed", TW_TYPE_FLOAT, &Renderer::getInstance()->tw_rotationSpeed,
			"step='0.01' max='5.0' min='0.02' label='Rotation Speed' group='Camera'");

	for (int i = 0; i < E_NUM_PROFILER_POINTS; i++) {
		tw_EProfilerPoint point = static_cast<tw_EProfilerPoint>(i);

		std::string label(tw_profilerPointNames[point]);
		TwAddVarRO(tw_tweakBar, ("profilerTiming" + std::to_string(i)).c_str(),
				TW_TYPE_DOUBLE, &Profiler::getInstance()->tw_profilerTimings[i],
				("label='" + label + "' precision='2' group='Profiling (ms)'").c_str());
	}

	TwAddVarRO(tw_tweakBar, "fps", TW_TYPE_DOUBLE, &Profiler::getInstance()->tw_fps,
			"label='Framerate (Hz)' precision='1' group='Profiling (ms)'");
	TwAddVarRW(tw_tweakBar, "profilerMeanSamples", TW_TYPE_INT32, &Profiler::getInstance()->tw_profilerMeanSamples,
			"step='1' max='1000' min='1' label='Samples' group='Profiling (ms)'");

	Profiler::getInstance()->init();

	glfwSwapInterval(1); // Disable v-sync.
}

void Context::afterDrawUpdate() {
	TwDraw();

	if (gMousePressed) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR & err != m_lastErr) {
		std::cout << "OpenGL error: " << err << std::endl;
		m_lastErr = err;
	}

	Profiler::getInstance()->afterDrawUpdate();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
	pollMouseAndKeyboardEvents(m_window);
}

void Context::printOpenGLInfo() {
	int GLFWmajor, GLFWminor, GLFWrev;

	GLint maxTexBufferSize, maxImgUnits, maxVertexImgUnits,
		maxImgUniforms, maxVertexImgUniforms, maxImgageUnits, maxTextureUnits;

	glfwGetVersion(&GLFWmajor, &GLFWminor, &GLFWrev);

	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTexBufferSize);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxImgUnits);
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexImgUnits);
	glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNIFORMS, &maxImgUniforms);
	glGetIntegerv(GL_MAX_VERTEX_IMAGE_UNIFORMS, &maxVertexImgUniforms);
	glGetIntegerv(GL_MAX_IMAGE_UNITS, &maxImgageUnits);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GLFW version %i.%i.%i\n", GLFWmajor, GLFWminor, GLFWrev);
	printf("GLEW version: %s\n\n", glewGetString(GLEW_VERSION));
	printf("Max. texture buffer size: %i\n", maxTexBufferSize);
	printf("Max. image combined texture image units: %i\n", maxImgUnits);
	printf("Max. vertex image units: %i\n", maxVertexImgUnits);
	printf("Max. image uniforms: %i\n", maxImgUniforms);
	printf("Max. vertex image uniforms: %i\n", maxVertexImgUniforms);
	printf("Max. image units: %i\n\n", maxImgageUnits);
	printf("Max. texture image units: %i\n\n", maxTextureUnits);
}
