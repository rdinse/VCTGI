#include <CVK_Framework/CVK_Framework.h>
#include "ShaderPhongShadow.h"
#include "SimpleCamera.h"

#define WIDTH 800
#define HEIGHT 800
#define SM_RES 1024

//define camera (trackball)
CVK::Trackball cam_trackball(WIDTH,HEIGHT);
//define camera for the spot light (fixed camera
SimpleCamera cam_shadowMapping(WIDTH,HEIGHT);

void resizeCallback( GLFWwindow *window, int w, int h)
{
	cam_trackball.setWidthHeight(w, h);
	cam_shadowMapping.setWidthHeight(w, h);
	glViewport(0, 0, w, h);
}

int main() 
{
	// init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shadow Mapping", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetWindowSizeCallback(window, resizeCallback);

	cam_trackball.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	cam_trackball.setRadius( 5);
	cam_trackball.setNearFar( 1.0f, 20.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);
	
	//allocate objects after rendering context initiation
	CVK::Teapot *teapot = new CVK::Teapot; 
	CVK::Sphere *sphere = new CVK::Sphere( 0.3f);
	CVK::Plane  *plane  = new CVK::Plane();

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/ShadowMapping/SpotLightShadows.vert", SHADERS_PATH "/ShadowMapping/SpotLightShadows.frag"};
	ShaderPhongShadow spotlightShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	const char *shadernamesSM[2] = {SHADERS_PATH "/ShadowMapping/ShadowMap.vert", SHADERS_PATH "/ShadowMapping/ShadowMap.frag"};
	CVK::ShaderMinimal shadowmapShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesSM);

	//define materials
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);
	CVK::Material mat_bronze( bronze, white, 100.f);

	//OpenGL parameters
	glClearColor(0.0, 0.53, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Main node in hierarchy
	CVK::Node* rootNode = new CVK::Node("Root");

	// Ground
	CVK::Node* groundNode = new CVK::Node("Ground");
	groundNode->setGeometry(plane);
	groundNode->setMaterial(&mat_bronze);
	groundNode->setModelMatrix(glm::scale( glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3(0,-3,0)), -90.0f, glm::vec3( 1, 0, 0)), glm::vec3( 10, 10, 10)));

	// Earth
	CVK::Node* earthNode = new CVK::Node("Earth");
	earthNode->setGeometry(teapot);
	earthNode->setMaterial(&mat_cvlogo);

	//Node in relation to earthNode
	CVK::Node* moonNode = new CVK::Node("Moon");
	moonNode->setGeometry(sphere);
	moonNode->setMaterial(&mat_brick);
	moonNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 3, 0, 0)));

	earthNode->addChild(moonNode);
	rootNode->addChild(earthNode);
	rootNode->addChild(groundNode);

	//define light
	CVK::Light slight( glm::vec4( 4, 8, 5, 1), darkgrey, glm::vec3(-1, -1, -1), 5.0f, glm::radians( 35.0f));
	CVK::State::getInstance()->addLight( &slight);

	//Set the position of the camera inside the light source
	glm::vec3 lightPosition = glm::vec3(*slight.getPosition());
	cam_shadowMapping.setPosition(lightPosition);

	//Set the lookAt point of the camera along the direction of the spot light
	glm::vec3 lookAt = lightPosition +  (*slight.getSpotDirection());
	cam_shadowMapping.setLookAt(lookAt);

	//Set the camera's field of view to the spot light's angle in degrees
	cam_shadowMapping.setFOV(glm::degrees(2*slight.getSpotCutoff()));

	//Set the cameras znear plane as far as possible and the zfar plane as near as possible to achieve the best quality
	//In general one would compute these values from the light's parameters
	cam_shadowMapping.setNearFar(1.f,30.f);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings( darkgrey, 0, white, 1, 10, 1);

	// Create a FrameBufferObject for the shadow mapping. Therefore no color attachement is needed - depth attachement only
	CVK::FBO sm_fbo(SM_RES, SM_RES, 0, true);

	double startTime = glfwGetTime();

	while( !glfwWindowShouldClose(window))
	{
		//delta time
		float deltaT = glfwGetTime() - startTime;
		startTime = glfwGetTime();

		//rotate in relation to time
		earthNode->setModelMatrix(glm::rotate(*earthNode->getModelMatrix(), 10.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		moonNode->setModelMatrix(glm::rotate(*moonNode->getModelMatrix(), 40.0f * deltaT, glm::vec3(1.0,1.0,1.0)));
		
		///////////////////////////////////////////////////////////////////
		//Draw scene from the lights point of view to create a shadow map//
		///////////////////////////////////////////////////////////////////
		{
			//Bind Shadow Map FBO
			sm_fbo.bind();

			//Set viewport
			glViewport(0,0,SM_RES,SM_RES);

			//Clear Shadow Map
			glClear(GL_DEPTH_BUFFER_BIT);

			//Update the camera
			CVK::State::getInstance()->setCamera( &cam_shadowMapping);
			cam_shadowMapping.update(window);

			//Set the shader for the light pass. This shader is highly optimized because the scene depth is the only thing that matters here!
			CVK::State::getInstance()->setShader(&shadowmapShader);
			shadowmapShader.update();

			//Render the scene
			rootNode->render();

			//Restore the default framebuffer
			sm_fbo.unbind();
		}

		//////////////
		//Draw Scene//
		//////////////

		//Set viewport
		glViewport(0,0,WIDTH,HEIGHT);

		//Clear Default Framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update Camera
		CVK::State::getInstance()->setCamera( &cam_trackball);
		cam_trackball.update( window);

		//Update shader
		CVK::State::getInstance()->setShader(&spotlightShader);

		//Update the spot light shader. As the shadow test is performed in "light space", the shader needs the MVP matrix of the light
		spotlightShader.update(&cam_shadowMapping,sm_fbo.getDepthTexture());

		//Render
		rootNode->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

