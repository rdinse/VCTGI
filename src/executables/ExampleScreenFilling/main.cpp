#include <CVK_Framework/CVK_Framework.h>

#define WIDTH 800
#define HEIGHT 800

#define WORLD 0
#define TEX1 1
#define TEX2 2

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

int mode = WORLD;

void charCallback (GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
		case '1':
			mode = WORLD;
			break;
		case '2':
			mode = TEX1;
			break;
		case '3':
			mode = TEX2;
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
	cam_trackball.setNearFar( 1.0f, 10.0f);
	CVK::State::getInstance()->setCamera( &cam_trackball);

	glfwSetCharCallback (window, charCallback);
	glfwSetWindowSizeCallback( window, resizeCallback);
	
	CVK::Teapot *teapot = new CVK::Teapot; 
	CVK::Sphere *sphere = new CVK::Sphere( 0.3f);
	
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	CVK::Node* earthNode = new CVK::Node("Earth");
	earthNode->setGeometry(teapot);
	earthNode->setMaterial(&mat_cvlogo);
	CVK::Node* moonNode = new CVK::Node("Moon");
	moonNode->setGeometry(sphere);
	moonNode->setMaterial(&mat_brick);
	moonNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 3, 0, 0)));
	earthNode->addChild(moonNode);
	
	//////////////////////////////////////////
	//////// FBO Creation ////////////////////
	//////////////////////////////////////////
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	CVK::FBO fbo(width, height, 1, true);
	
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);
	const char *shadernamesSimpleTexture[2] = {SHADERS_PATH "/Examples/screenFill.vert", SHADERS_PATH "/Examples/simpleTexture.frag"};
	CVK::ShaderSimpleTexture simpleTextureShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesSimpleTexture);

	CVK::Light plight( glm::vec4( -1, 1, 1, 1), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &plight);

	double startTime = glfwGetTime();

	while( !glfwWindowShouldClose( window))
	{
		float deltaT = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		earthNode->setModelMatrix(glm::rotate(*earthNode->getModelMatrix(), 10.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		moonNode->setModelMatrix(glm::rotate(*moonNode->getModelMatrix(), 40.0f * deltaT, glm::vec3(1.0,1.0,1.0)));
		
		//////////////////////////////////////////
		//////// FBO activation //////////////////
		//////////////////////////////////////////
		fbo.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cam_trackball.update( window);
		CVK::State::getInstance()->updateSceneSettings( darkgrey, FOG_LINEAR, white, 1, 10, 1);
		CVK::State::getInstance()->setShader(&phongShader);
		phongShader.update();
		earthNode->render();
		
		//////////////////////////////////////////
		//////// FBO visualization ///////////////
		//////////////////////////////////////////
		fbo.unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if( mode==WORLD)
			simpleTextureShader.setTextureInput(0, fbo.getColorTexture(0));
		else if( mode==TEX1)
			simpleTextureShader.setTextureInput(0, mat_cvlogo.getTexture(CVK::COLOR_TEXTURE)->getTexture());
		else
			simpleTextureShader.setTextureInput(0, mat_brick.getTexture(CVK::COLOR_TEXTURE)->getTexture());

		simpleTextureShader.useProgram();
		simpleTextureShader.update();
		simpleTextureShader.render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

