#include <CVK_Framework/CVK_Framework.h>

#define WIDTH 800
#define HEIGHT 800

GLFWwindow* window;

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

float shininess = 100.0f;
float alpha = 0.9f;

void charCallback (GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
		case '+':
			alpha += 0.02f;
			if (alpha > 1)
				alpha = 1;
			break;
		case '-':
			alpha -= 0.02f;
			if (alpha < 0)
				alpha = 0;
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
	window = glfwCreateWindow(WIDTH, HEIGHT, "CVK Stencil Buffer Example", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);

	glewInit();

	cam_trackball.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	cam_trackball.setRadius( 5);
	cam_trackball.setNearFar( 1.0f, 10.0f);

	glfwSetCharCallback (window, charCallback);
	glfwSetWindowSizeCallback( window, resizeCallback);
	
	CVK::Teapot *teapot = new CVK::Teapot; 
	CVK::Plane *plane = new CVK::Plane;
	CVK::Sphere *sphere = new CVK::Sphere( 0.3f);
	CVK::Cone *cone = new CVK::Cone( 0.2f, 0.2f, 0.5f);

	//load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/Examples/Phong.vert", SHADERS_PATH "/Examples/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);

	// define materials
	CVK::Material mat_bronze_transp( bronze, white, shininess);
	CVK::Material mat_brick((char*)RESOURCES_PATH "/brick.bmp", black, darkgrey, shininess);
	CVK::Material mat_cvlogo((char*)RESOURCES_PATH "/cv_logo.bmp", black, grey, shininess);
	CVK::Material mat_red (red, white, shininess);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_STENCIL_TEST);
	
	CVK::Node* scene = new CVK::Node("Scene");

	CVK::Node* sphereNode = new CVK::Node("Sphere");
	sphereNode->setGeometry(sphere);
	sphereNode->setMaterial(&mat_brick);
	sphereNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 2, 0.4, -2)));
	CVK::Node* sphereNode2 = new CVK::Node();
	sphereNode2->setGeometry(sphere);
	sphereNode2->setModelMatrix(glm::rotate(glm::translate(glm::mat4( 1.0f), glm::vec3( 0, -0.8, 0)), 180.0f, glm::vec3( 1, 0, 0)));

	CVK::Node* coneNode = new CVK::Node("Cone");
	coneNode->setGeometry(cone);
	coneNode->setMaterial(&mat_red);
	coneNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( -2, 0, -2)));
	CVK::Node* coneNode2 = new CVK::Node();
	coneNode2->setGeometry(cone);
	coneNode2->setModelMatrix(glm::rotate( glm::mat4(1.0), 180.0f, glm::vec3( 1, 0, 0)));

	CVK::Node* teapotNode = new CVK::Node("Teapot");
	teapotNode->setGeometry(teapot);
	teapotNode->setMaterial(&mat_cvlogo);
	teapotNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 0, 0.72, 0)));
	CVK::Node* teapotNode2 = new CVK::Node();
	teapotNode2->setGeometry(teapot);
	teapotNode2->setModelMatrix(glm::rotate(glm::translate(glm::mat4( 1.0f), glm::vec3( 0, -1.44, 0)), 180.0f, glm::vec3( 1, 0, 0)));
	
	CVK::Node* planeNode = new CVK::Node("Plane");
	planeNode->setGeometry(plane);
	planeNode->setMaterial(&mat_bronze_transp);
	planeNode->setModelMatrix(glm::scale( glm::rotate( glm::mat4( 1.0f), -90.0f, glm::vec3( 1, 0, 0)), glm::vec3( 3, 3, 3)));
	
	scene->addChild(sphereNode);
	sphereNode->addChild(sphereNode2);
	scene->addChild(coneNode);
	coneNode->addChild(coneNode2);
	scene->addChild(teapotNode);
	teapotNode->addChild(teapotNode2);
	scene->addChild(planeNode);
	
	CVK::Node* sceneStencil = new CVK::Node("SceneStencil");

	CVK::Node* stencilSphereNode = new CVK::Node("Sphere");
	stencilSphereNode->setGeometry(sphere);
	stencilSphereNode->setMaterial(&mat_brick);
	stencilSphereNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 2, 0.4, -2)));

	CVK::Node* stencilConeNode = new CVK::Node("Cone");
	stencilConeNode->setGeometry(cone);
	stencilConeNode->setMaterial(&mat_red);
	stencilConeNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( -2, 0, -2)));

	CVK::Node* stencilTeapotNode = new CVK::Node("Teapot");
	stencilTeapotNode->setGeometry(teapot);
	stencilTeapotNode->setMaterial(&mat_cvlogo);
	stencilTeapotNode->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 0, 0.72, 0)));
	
	CVK::Node* stencilPlaneNode = new CVK::Node("Plane");
	stencilPlaneNode->setGeometry(plane);
	stencilPlaneNode->setMaterial(&mat_bronze_transp);
	stencilPlaneNode->setModelMatrix(glm::scale( glm::rotate( glm::mat4( 1.0f), -90.0f, glm::vec3( 1, 0, 0)), glm::vec3( 2, 2, 2)));

	sceneStencil->addChild(stencilSphereNode);
	sceneStencil->addChild(stencilConeNode);
	sceneStencil->addChild(stencilTeapotNode);
	sceneStencil->addChild(stencilPlaneNode);
	
	CVK::State::getInstance()->setCamera( &cam_trackball);

	CVK::Light dlight( glm::vec4( -1, 1, 1, 0), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight( &dlight);
	
	//for fbo
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	CVK::FBO fbo(width, height, 1, false, true);

	//fbo.bind();

	const char *shadernamesSimpleTexture[2] = {SHADERS_PATH "/Examples/screenFill.vert", SHADERS_PATH "/Examples/simpleTexture.frag"};
	CVK::ShaderSimpleTexture simpleTextureShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernamesSimpleTexture);

	while( !glfwWindowShouldClose( window))
	{
		CVK::State::getInstance()->setCamera( &cam_trackball);

		//fbo activation
		fbo.bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		coneNode->setModelMatrix(glm::translate(*coneNode->getModelMatrix(), glm::vec3(0.001,0.0,0.0)));
		stencilConeNode->setModelMatrix(glm::translate(*stencilConeNode->getModelMatrix(), glm::vec3(0.001,0.0,0.0)));
		
		//Update Camera
		cam_trackball.update( window);

		//define Scene uniforms (ambient and fog)
		CVK::State::getInstance()->updateSceneSettings( darkgrey, FOG_LINEAR, white, 1, 10, 1);
		CVK::State::getInstance()->setShader(&phongShader);
		mat_bronze_transp.setAlpha( alpha);
		phongShader.update();

		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glClearStencil(1);
		glClear(GL_STENCIL_BUFFER_BIT);
		sceneStencil->render();

		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0x00);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		scene->render();

		//for fbo visualization
		fbo.unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		simpleTextureShader.setTextureInput(0, fbo.getColorTexture(0));
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

