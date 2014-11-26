#include <CVK_Framework/CVK_Framework.h>

#include "ShaderNormalMapping.h"

#define WIDTH 800
#define HEIGHT 800

//define camera (trackball)
CVK::Trackball camera(WIDTH, HEIGHT);
bool useNormalMappingShader;
bool useColorTexture;

void resizeCallback( GLFWwindow *window, int w, int h)
{
	camera.setWidthHeight(w, h);
	glViewport(0, 0, w, h);
}

void charCallback(GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
		case 'm':
			useColorTexture = !useColorTexture;
			break;
		case 's':
			useNormalMappingShader = !useNormalMappingShader;
			break;
	}
}

int main() 
{
	// init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Normal Mapping", 0, 0);
	glfwSetWindowPos(window, 100, 50);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetCharCallback(window, charCallback);

	// setup camera
	camera.setCenter(glm::vec3( 0.0f, 0.0f, 0.0f));
	camera.setRadius(5);
	camera.setNearFar(1.0f, 10.0f);
	CVK::State::getInstance()->setCamera(&camera);

	// setup light
	CVK::Light plight(glm::vec4(0.0f, 1.0, 10.0f, 1.0f), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight(&plight);

	// setup scene
	CVK::Node* node = new CVK::Node("Cube");
	// create and set teapot geometrie
	CVK::Cube* cube = new CVK::Cube(); 
	node->setGeometry(cube);
	// define material with diffuse color and normal texture
	CVK::Material matTex((char*)RESOURCES_PATH "/normalmapping/diffusemap.png", glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3( 0.3f, 0.3f, 0.3f), 120.0f);
	matTex.setTexture(CVK::NORMAL_TEXTURE, (char*)RESOURCES_PATH "/normalmapping/normalmap.png");
	CVK::Material matRed(glm::vec3(1.0,0.0,0.0), glm::vec3(1.0,1.0,1.0), 120.0f);
	useColorTexture = false;

	// phong shader
	const char *shadernames0[2] = {SHADERS_PATH "/NormalMapping/Phong.vert", SHADERS_PATH "/NormalMapping/Phong.frag"};
 	CVK::ShaderPhong phongShader = CVK::ShaderPhong( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames0);
 	//define Scene uniforms (ambient and fog)
	CVK::State::getInstance()->updateSceneSettings(glm::vec3(0.3,0.3,0.3), 0, glm::vec3(1.0,1.0,1.0), 1, 10, 1);
	// normal mapping shader
	const char *shadernames1[2] = {SHADERS_PATH "/NormalMapping/NormalMapping.vert", SHADERS_PATH "/NormalMapping/NormalMapping.frag"};
	ShaderNormalMapping normalMappingShader = ShaderNormalMapping( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames1);
	//use normal mapping shader
	useNormalMappingShader = false;

	// print infos
	std::cout << "Key s: " << "swap shader" << std::endl;
	std::cout << "Key m: " << "swap material" << std::endl;

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	while( !glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// set active shader
		if(useNormalMappingShader)
			CVK::State::getInstance()->setShader(&normalMappingShader);
		else
			CVK::State::getInstance()->setShader(&phongShader);

		// set material
		if(useColorTexture)
			node->setMaterial(&matTex);
		else
			node->setMaterial(&matRed);
		
		// update camera
		camera.update(window);
		// update view, projection matrix, light uniforms
		CVK::State::getInstance()->getShader()->update();
		// render scene
		node->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	// clean up
	delete cube;
	delete node;

	return 0;
}

