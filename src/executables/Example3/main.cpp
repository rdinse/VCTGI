#include <CVK_Framework/CVK_Framework.h>

#define WIDTH 800
#define HEIGHT 800

int modelNumber = 0;
std::vector<CVK::Node*> nodeList;

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
		case 'n':
			modelNumber = (modelNumber + 1) % nodeList.size();
			break;
		case 'N':
			modelNumber = (modelNumber + nodeList.size() - 1) % nodeList.size();
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
	glfwSetCharCallback (window, charCallback);
	
	cam_trackball.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	cam_trackball.setRadius( 5);
	cam_trackball.setNearFar( 1.0f, 10.0f);

	glfwSetWindowSizeCallback( window, resizeCallback);

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	// define materials
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, 100.0f);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, 100.0f);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	
	CVK::Node* scene = new CVK::Node("Scene", RESOURCES_PATH "/cars/max/cars.3DS");
	scene->setModelMatrix(glm::scale( glm::rotate( glm::mat4(1.0f), -90.0f, glm::vec3( 1, 0, 0)), glm::vec3( 0.0008, 0.0008, 0.0008)));

	nodeList.push_back(scene);
	for(unsigned int i=0; i<scene->getChilds()->size(); i++)
	{
		nodeList.push_back(scene->getChilds()->at(i));
	}
	
	CVK::State::getInstance()->setCamera( &cam_trackball);

	CVK::Light plight( glm::vec4( -1, 1, 1, 1), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &plight);

	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings( darkgrey, 0, white, 1, 10, 1);
	
	CVK::State::getInstance()->setShader(&phongShader);

	while( !glfwWindowShouldClose( window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Update Camera
		cam_trackball.update( window);

		phongShader.update();
		nodeList[modelNumber]->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

