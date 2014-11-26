#include <CVK_Framework/CVK_Framework.h>

#include "ShaderGBuffer.h"
#include "ShaderCompositing.h"
#include "ShaderLighting.h"

#define WIDTH 800
#define HEIGHT 800

#define POSITION 0
#define NORMAL 1
#define COLOR 2
#define LIGHT 3
#define FINAL 4

#define OBJECTS 0
#define PLANE 1

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

int mode = FINAL;
int toRender = OBJECTS;

void charCallback (GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
		case '1':
			mode = POSITION;
			break;
		case '2':
			mode = NORMAL;
			break;
		case '3':
			mode = COLOR;
			break;
		case '4':
			mode = LIGHT;
			break;
		case '5':
			mode = FINAL;
			break;
		case 'o':
			toRender = OBJECTS;
			break;
		case 'p':
			toRender = PLANE;
			break;
	}
}

void resizeCallback( GLFWwindow *window, int w, int h)
{
	cam_trackball.setWidthHeight( w, h);
	glViewport( 0, 0, w, h);
}

int main() 
{
	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CVK Example", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);

	glewInit();

	cam_trackball.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	cam_trackball.setRadius( 5);
	cam_trackball.setNearFar( 0.1f, 100.0f);

	glfwSetCharCallback (window, charCallback);
	glfwSetWindowSizeCallback( window, resizeCallback);
	
	//allocate objects after rendering context initiation
	CVK::Teapot *teapot = new CVK::Teapot; 
	CVK::Sphere *sphere = new CVK::Sphere( 1.0f);
	CVK::Plane *plane = new CVK::Plane();

	CVK::Sphere *lightSphere = new CVK::Sphere( 3.0f);
	
	const char *shadernamesGBuffer[2] = {SHADERS_PATH "/DeferredShading/GBuffer.vert", SHADERS_PATH "/DeferredShading/GBuffer.frag"};
	ShaderGBuffer gBufferShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesGBuffer);
	
	//define materials
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	
	int numObjects = 5;
	CVK::Node* objects = new CVK::Node("Objects");
	for(int i=-numObjects; i<=numObjects; i++)
		for(int j=-numObjects; j<=numObjects; j++)
		{
			CVK::Node* object = new CVK::Node("Object");
			object->setGeometry(sphere);
			object->setMaterial(&mat_cvlogo);
			object->setModelMatrix(glm::scale( glm::translate( glm::mat4( 1.0f), glm::vec3( i*1.2, -2, j*1.2)), glm::vec3(0.5,0.5,0.5)));
			objects->addChild(object);
		}

	int numLights = 5;
	CVK::Node* lights = new CVK::Node("Lights");
	for(int i=-numLights; i<=numLights; i++)
		for(int j=-numLights; j<=numLights; j++)
		{
			CVK::Node* light = new CVK::Node("Light");
			light->setGeometry(lightSphere);
			light->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( i*1.2, -1, j*1.2)));
			lights->addChild(light);
		}

	CVK::Node* planeNode = new CVK::Node("Plane");
	planeNode->setGeometry(plane);
	planeNode->setMaterial(&mat_cvlogo);
	planeNode->setModelMatrix(glm::rotate(
		glm::scale( glm::translate( glm::mat4( 1.0f), glm::vec3(0,-2,0)), glm::vec3(numLights+2)), -90.0f, glm::vec3(1,0,0)));
	
	CVK::State::getInstance()->setCamera( &cam_trackball);
	
	//for fbo
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	width = WIDTH;
	height = HEIGHT;
	
	CVK::FBO fbo(width, height, 3, true);
	const char *shadernamesSimpleTexture[2] = {SHADERS_PATH "/DeferredShading/screenFill.vert", SHADERS_PATH "/DeferredShading/simpleTexture.frag"};
	CVK::ShaderSimpleTexture simpleTextureShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesSimpleTexture);
	
	CVK::FBO fboLighting(width, height, 1, true);
	const char *shadernamesLighting[2] = {SHADERS_PATH "/DeferredShading/lighting.vert", SHADERS_PATH "/DeferredShading/lighting.frag"};
	ShaderLighting lightingShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesLighting);
	lightingShader.setTextureInput(0, fbo.getColorTexture(0));
	lightingShader.setTextureInput(1, fbo.getColorTexture(1));
	lightingShader.setTextureInput(2, fbo.getColorTexture(2));
	
	const char *shadernamesCompositing[2] = {SHADERS_PATH "/DeferredShading/screenFill.vert", SHADERS_PATH "/DeferredShading/finalCompositing.frag"};
	ShaderCompositing compositingShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesCompositing);
	compositingShader.setTextureInput(0, fboLighting.getColorTexture(0));
	compositingShader.setTextureInput(1, fbo.getColorTexture(2));

	double startTime = glfwGetTime();

	while( !glfwWindowShouldClose( window))
	{
		//delta time
		float deltaT = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		
		lights->setModelMatrix(glm::rotate(*lights->getModelMatrix(), 10.0f * deltaT, glm::vec3(0.0,1.0,0.0)));


		//fbo activation
		fbo.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam_trackball.update( window);
		CVK::State::getInstance()->updateSceneSettings( darkgrey, FOG_LINEAR, white, 1, 10, 1);
		CVK::State::getInstance()->setShader(&gBufferShader);
		gBufferShader.update();

		if (toRender == OBJECTS) 
			objects->render();
		else 
			planeNode->render();

		//fbo for lights activation
		fboLighting.bind();
		
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glClearColor(0,0,0,0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CVK::State::getInstance()->setShader(&lightingShader);
		lightingShader.update();

		lights->render();
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClearColor(1.0, 1.0, 1.0, 0.0);

		//for fbo visualization
		fbo.unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(mode < 3)
		{
			simpleTextureShader.useProgram();
			simpleTextureShader.setTextureInput(0, fbo.getColorTexture(mode));
			simpleTextureShader.update();
			simpleTextureShader.render();
		}
		else if(mode == 3)
		{
			simpleTextureShader.useProgram();
			simpleTextureShader.setTextureInput(0, fboLighting.getColorTexture(0));
			simpleTextureShader.update();
			simpleTextureShader.render();
		}
		else
		{
			compositingShader.useProgram();
			compositingShader.update();
			compositingShader.render();
		}

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

