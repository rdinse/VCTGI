#include <CVK_Framework/CVK_Framework.h>

#define WIDTH 800
#define HEIGHT 800

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

CVK::Node* shoulderNode;
CVK::Node* elbowNode;

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
		case 'E':
			elbowNode->setModelMatrix(glm::rotate(*elbowNode->getModelMatrix(), -1.0f, glm::vec3( 0, 0, 1)));
			break;
		case 'e':
			elbowNode->setModelMatrix(glm::rotate(*elbowNode->getModelMatrix(), 1.0f, glm::vec3( 0, 0, 1)));
			break;
		case 'S':
			shoulderNode->setModelMatrix(glm::rotate(*shoulderNode->getModelMatrix(), -1.0f, glm::vec3( 0, 0, 1)));
			break;
		case 's':
			shoulderNode->setModelMatrix(glm::rotate(*shoulderNode->getModelMatrix(), 1.0f, glm::vec3( 0, 0, 1)));
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
	glfwSetCharCallback (window, charCallback);

	glewInit();
	
	cam_trackball.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	cam_trackball.setRadius( 5);
	cam_trackball.setNearFar( 1.0f, 10.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);
	
	//allocate objects after rendering context initiation
	CVK::Cube *cube = new CVK::Cube(0.5f);

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	//define materials
	CVK::Material mat_upperArm(glm::vec3(0.6f), glm::vec3(0.6f), 100.0f);
	CVK::Material mat_foreArm(glm::vec3(0.8f), glm::vec3(0.8f), 100.0f);

	//OpenGL parameters
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
		sceneNode
			|
			|
		shoulderNode (for shoulder rotation)
			|			\
			|			 \
		upperArmNode	elbowNode (for elbow rotation)
			|				|
			|				|
		upperArmGeom	foreArmNode
							|
							|
						foreArmGeom
	*/

	//Main node in hierarchy
	CVK::Node* sceneNode = new CVK::Node("Scene");
	sceneNode->setModelMatrix( glm::translate(glm::mat4( 1.0f), glm::vec3( -1, 0, 0)));

	//Node for shoulder
	shoulderNode = new CVK::Node("Shoulder");
	sceneNode->addChild(shoulderNode);

	//Node for upper arm
	CVK::Node* upperArmNode = new CVK::Node("Upperarm");
	upperArmNode->setGeometry(cube);
	upperArmNode->setMaterial(&mat_upperArm);
	upperArmNode->setModelMatrix(glm::scale( glm::translate(glm::mat4( 1.0f), glm::vec3( 1, 0, 0)), glm::vec3( 2.0, 0.4, 1.01)));
	shoulderNode->addChild(upperArmNode);

	//Node for elbow
	elbowNode = new CVK::Node("Elbow");
	elbowNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 2, 0, 0)));
	shoulderNode->addChild(elbowNode);

	//Node for fore arm
	CVK::Node* foreArmNode = new CVK::Node("Forearm");
	foreArmNode->setGeometry(cube);
	foreArmNode->setMaterial(&mat_foreArm);
	foreArmNode->setModelMatrix(glm::scale( glm::translate(glm::mat4( 1.0f), glm::vec3( 0.75, 0, 0)), glm::vec3( 1.5, 0.4, 1.0)));
	elbowNode->addChild(foreArmNode);
	
	CVK::State::getInstance()->setCamera( &cam_trackball);

	//define light
	CVK::Light plight( glm::vec4( -1, 1, 1, 0), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &plight);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings( glm::vec3(0.5f), 0, white, 1, 10, 1);
	CVK::State::getInstance()->setShader(&phongShader);

	double startTime = glfwGetTime();

	while( !glfwWindowShouldClose( window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Update Camera
		cam_trackball.update( window);

		//update shader and render
		phongShader.update();
		sceneNode->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

