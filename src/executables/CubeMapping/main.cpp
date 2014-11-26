#include <CVK_Framework/CVK_Framework.h>
#include "ShaderSkyBox.h"
#include "ShaderCubeMapReflection.h"

#define WIDTH 800
#define HEIGHT 800

float reflection = 0.7f;
bool showSkyBox = true;

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

void resizeCallback( GLFWwindow *window, int w, int h)
{
	cam_trackball.setWidthHeight( w, h);
	glViewport( 0, 0, w, h);
}

void charCallback (GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
		case 'W':
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			break;
		case 'w':
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			break;
		case '+':
			reflection += 0.1f;
			if (reflection > 1)
				reflection = 1;
			break;
		case '-':
			reflection -= 0.1f;
			if (reflection < 0)
				reflection = 0;
			break;
		case 's':
			showSkyBox = !showSkyBox;
			break;
	}
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
	cam_trackball.setNearFar( 0.1f, 10.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);
	glfwSetCharCallback (window, charCallback);
	
	//allocate objects after rendering context initiation
	CVK::Teapot *teapot = new CVK::Teapot; 
	CVK::Sphere *sphere = new CVK::Sphere( 0.3f);
	CVK::Cube *cube = new CVK::Cube(1.0f);

	//load cube map texture
	const char *textureNames[6] = {(char*)RESOURCES_PATH "/cubeMap/posx.jpg",
									(char*)RESOURCES_PATH "/cubeMap/negx.jpg",
									(char*)RESOURCES_PATH "/cubeMap/posy.jpg",
									(char*)RESOURCES_PATH "/cubeMap/negy.jpg",
									(char*)RESOURCES_PATH "/cubeMap/posz.jpg",
									(char*)RESOURCES_PATH "/cubeMap/negz.jpg" };
	CVK::CubeMapTexture cubeMap( textureNames);
	
	const char *skyBoxShadernames[2] = {SHADERS_PATH "/CubeMapping/SkyBox.vert", SHADERS_PATH "/CubeMapping/SkyBox.frag"};
	ShaderSkyBox skyboxShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, skyBoxShadernames);

	const char *reflectionShadernames[2] = {SHADERS_PATH "/CubeMapping/CubeMapReflection.vert", SHADERS_PATH "/CubeMapping/CubeMapReflection.frag"};
	ShaderCubeMapReflection reflectionShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, reflectionShadernames);

	//define materials
	CVK::Material mat_blue(blue, white, 100.0f);
	CVK::Material mat_red(red, white, 100.0f);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Main node in hierarchy
	CVK::Node* earthNode = new CVK::Node("Earth");
	earthNode->setGeometry(teapot);
	earthNode->setMaterial(&mat_blue);

	//Node in relation to earthNode
	CVK::Node* moonNode = new CVK::Node("Moon");
	moonNode->setGeometry(sphere);
	moonNode->setMaterial(&mat_red);
	moonNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 3, 0, 0)));

	earthNode->addChild(moonNode);
	
	CVK::State::getInstance()->setCamera( &cam_trackball);

	//define light
	CVK::Light plight( glm::vec4( 0, 1, 1, 0), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &plight);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings( grey, 0, white, 1, 10, 1);

	double startTime = glfwGetTime();

	CVK::State::getInstance()->setCubeMapTexture(&cubeMap);

	while( !glfwWindowShouldClose( window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//delta time
		float deltaT = glfwGetTime() - startTime;
		startTime = glfwGetTime();

		//rotate in relation to time
		earthNode->setModelMatrix(glm::rotate(*earthNode->getModelMatrix(), 10.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		moonNode->setModelMatrix(glm::rotate(*moonNode->getModelMatrix(), 40.0f * deltaT, glm::vec3(1.0,1.0,1.0)));
		
		//update materials
		mat_blue.setReflectionStrength(reflection);
		mat_red.setReflectionStrength(reflection);
		
		//Update Camera
		cam_trackball.update( window);

		if(showSkyBox)
		{
			glDisable(GL_DEPTH_TEST);
			CVK::State::getInstance()->setShader(&skyboxShader);
			skyboxShader.update();
			cube->render();
			glEnable(GL_DEPTH_TEST);
		}
		
		//update shader and render
		CVK::State::getInstance()->setShader(&reflectionShader);
		reflectionShader.update();
		earthNode->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

