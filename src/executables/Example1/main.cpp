#include <CVK_Framework/CVK_Framework.h>

#define WIDTH 800
#define HEIGHT 800

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

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
	cam_trackball.setNearFar( 1.0f, 10.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);
	
	//allocate objects after rendering context initiation
	CVK::Teapot *teapot = new CVK::Teapot; 
	CVK::Sphere *sphere = new CVK::Sphere( 0.3f);

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	//define materials
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Main node in hierarchy
	CVK::Node* earthNode = new CVK::Node("Earth");
	earthNode->setGeometry(teapot);
	earthNode->setMaterial(&mat_cvlogo);

	//Node in relation to earthNode
	CVK::Node* moonNode = new CVK::Node("Moon");
	moonNode->setGeometry(sphere);
	moonNode->setMaterial(&mat_brick);
	moonNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 3, 0, 0)));

	earthNode->addChild(moonNode);
	
	CVK::State::getInstance()->setCamera( &cam_trackball);

	//define light
	CVK::Light plight( glm::vec4( -1, 1, 1, 1), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &plight);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings( darkgrey, 0, white, 1, 10, 1);
	CVK::State::getInstance()->setShader(&phongShader);

	double startTime = glfwGetTime();

	while( !glfwWindowShouldClose( window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//delta time
		float deltaT = glfwGetTime() - startTime;
		startTime = glfwGetTime();

		//rotate in relation to time
		earthNode->setModelMatrix(glm::rotate(*earthNode->getModelMatrix(), 10.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		moonNode->setModelMatrix(glm::rotate(*moonNode->getModelMatrix(), 40.0f * deltaT, glm::vec3(1.0,1.0,1.0)));
		
		//Update Camera
		cam_trackball.update( window);

		//update shader and render
		phongShader.update();
		earthNode->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

