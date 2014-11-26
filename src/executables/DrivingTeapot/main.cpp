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
	CVK::Cone *cone = new CVK::Cone(0.26,0.26,0.4);

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	//define materials
	CVK::Material mat_black((char*)RESOURCES_PATH "/wheel.bmp", black, grey, 20.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Main node in hierarchy
	CVK::Node* cars = new CVK::Node("Cars");

	//Node in relation to carNode
	CVK::Node* wheel1Node = new CVK::Node("Wheel1");
	wheel1Node->setGeometry(cone);
	wheel1Node->setMaterial(&mat_black);
	wheel1Node->setModelMatrix(glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( 0.5, -0.6, 0.7)), 90.0f, glm::vec3(1,0,0)));
	CVK::Node* wheel2Node = new CVK::Node("Wheel2");
	wheel2Node->setGeometry(cone);
	wheel2Node->setMaterial(&mat_black);
	wheel2Node->setModelMatrix(glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( -0.5, -0.6, 0.7)), 90.0f, glm::vec3(1,0,0)));
	CVK::Node* wheel3Node = new CVK::Node("Wheel3");
	wheel3Node->setGeometry(cone);
	wheel3Node->setMaterial(&mat_black);
	wheel3Node->setModelMatrix(glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( -0.5, -0.6, -1.1)), 90.0f, glm::vec3(1,0,0)));
	CVK::Node* wheel4Node = new CVK::Node("Wheel4");
	wheel4Node->setGeometry(cone);
	wheel4Node->setMaterial(&mat_black);
	wheel4Node->setModelMatrix(glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( 0.5, -0.6, -1.1)), 90.0f, glm::vec3(1,0,0)));
	
	int numCars = 8;
	for(unsigned int i=0; i<numCars; i++)
	{
		//One car
		CVK::Node* carNode = new CVK::Node("Car");
		carNode->setGeometry(teapot);
		carNode->setMaterial(&mat_cvlogo);
		carNode->setModelMatrix(glm::scale( 
									glm::translate( 
										glm::rotate( 
											glm::mat4(1.0f), 
										360.0f*i/numCars, glm::vec3(0,1,0)), 
									glm::vec3(0,0,-3)), 
								glm::vec3(4.0f/numCars)));

		carNode->addChild(wheel1Node);
		carNode->addChild(wheel2Node);
		carNode->addChild(wheel3Node);
		carNode->addChild(wheel4Node);

		cars->addChild(carNode);
	}
	
	CVK::State::getInstance()->setCamera( &cam_trackball);

	//define light
	CVK::Light dlight( glm::vec4( -1, 1, 1, 1), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &dlight);

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
		
		cars->setModelMatrix(glm::rotate(*cars->getModelMatrix(), -8.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		wheel1Node->setModelMatrix(glm::rotate(*wheel1Node->getModelMatrix(), -100.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		wheel2Node->setModelMatrix(glm::rotate(*wheel2Node->getModelMatrix(), -100.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		wheel3Node->setModelMatrix(glm::rotate(*wheel3Node->getModelMatrix(), -100.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		wheel4Node->setModelMatrix(glm::rotate(*wheel4Node->getModelMatrix(), -100.0f * deltaT, glm::vec3(0.0,1.0,0.0)));
		
		//Update Camera
		cam_trackball.update( window);

		//update shader and render
		phongShader.update();
		cars->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

