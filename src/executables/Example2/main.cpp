#include <CVK_Framework/CVK_Framework.h>

#define WIDTH 800
#define HEIGHT 800

#define PLIGHT 0 //Point light
#define DLIGHT 1 //Directional Light
#define SLIGHT 2 //Spot Light

GLFWwindow* window;

//define Objects
CVK::Teapot *teapot; //OpenGL context is needed for construction
CVK::Plane *plane;
CVK::Sphere *sphere;
CVK::Cone *cone;

//define camera (trackball)
CVK::Trackball cam_trackball( WIDTH, HEIGHT);

int lmode = PLIGHT;
float shininess = 100.0f;
float alpha = 0.9f;

CVK::Light plight( glm::vec4( -1, 1, 1, 1), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
CVK::Light dlight( glm::vec4( -1, 1, 1, 0), grey, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
CVK::Light slight( glm::vec4( -1, 1, -1, 1), darkgrey, glm::vec3(-1, -1, -1), 5.0f, glm::radians( 35.0f));

void PrintInfo()
{
	printf("w/W:	Wireframe/Fill\n");
	printf("1:		Point Light\n");
	printf("2:		Directional Light\n");
	printf("3:		Spot Light\n");
	printf("+/-:	decrease/increase Transparency\n\n");
}

void OpenGL_Init()
{
	//Camera
	cam_trackball.setCenter( glm::vec3( 0.0f, 0.0f, 0.0f));
	cam_trackball.setRadius( 5);
	cam_trackball.setNearFar( 1.0f, 10.0f);
}

void Camera_update()
{
	cam_trackball.update( window);
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
		case '1':
			lmode = PLIGHT;
			break;
		case '2':
			lmode = DLIGHT;
			break;
		case '3':
			lmode = SLIGHT;
			break;
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
	PrintInfo();

	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	window = glfwCreateWindow(WIDTH, HEIGHT, "CVK Example", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);


	glewInit();
	OpenGL_Init();

	glfwSetCharCallback (window, charCallback);
	glfwSetWindowSizeCallback( window, resizeCallback);

	//allocate objects after rendering context initiation
	teapot = new CVK::Teapot; 
	plane = new CVK::Plane;
	sphere = new CVK::Sphere( 0.3f);
	cone = new CVK::Cone( 0.2f, 0.2f, 0.5f);

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
	planeNode->setModelMatrix(glm::scale( glm::rotate( glm::mat4( 1.0f), -90.0f, glm::vec3( 1, 0, 0)), glm::vec3( 5, 5, 5)));
	
	CVK::Node* geomNode = new CVK::Node("Geom");
	//CVK::Node* geomNode = new CVK::Node("Geom", RESOURCES_PATH "/cars/max/cars.obj");
	geomNode->setModelMatrix(glm::scale( glm::rotate( glm::translate(glm::mat4(1.0f), glm::vec3(0,0,2)), -90.0f, glm::vec3( 1, 0, 0)), glm::vec3( 0.0008, 0.0008, 0.0008)));
	scene->addChild(geomNode);

	scene->addChild(sphereNode);
	sphereNode->addChild(sphereNode2);
	scene->addChild(coneNode);
	coneNode->addChild(coneNode2);
	scene->addChild(teapotNode);
	teapotNode->addChild(teapotNode2);
	scene->addChild(planeNode);
	
	CVK::State::getInstance()->setCamera( &cam_trackball);
	CVK::State::getInstance()->addLight( &plight);
	
	//for fbo
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	CVK::FBO fbo(width, height, 1, true);

	//fbo.bind();

	//screen filling quad setttings
	CVK::Node* screenFillingQuad = new CVK::Node("Screen-Filling Quad");
	screenFillingQuad->setGeometry(plane);
	CVK::Material mat_screen( fbo.getColorTexture(0), black, grey, shininess);
	screenFillingQuad->setMaterial(&mat_screen);
	
	CVK::State::getInstance()->setShader(&phongShader);

	while( !glfwWindowShouldClose( window))
	{
		CVK::State::getInstance()->setCamera( &cam_trackball);

		//fbo activation
		fbo.bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CVK::Node* node = scene->find("Cone");
		if (node) 
			node->setModelMatrix(glm::translate(*node->getModelMatrix(), glm::vec3(0.001,0.0,0.0)));
		
		//Update Camera
		Camera_update();

		if (lmode == PLIGHT)
			CVK::State::getInstance()->setLight( 0, &plight);
		else if (lmode == DLIGHT)
			CVK::State::getInstance()->setLight( 0, &dlight);
		else if (lmode == SLIGHT)
			CVK::State::getInstance()->setLight( 0, &slight);

		//define Scene uniforms (ambient and fog)
		CVK::State::getInstance()->updateSceneSettings( darkgrey, FOG_LINEAR, white, 1, 10, 1);
		
		mat_bronze_transp.setAlpha( alpha);
		
		phongShader.update();

		scene->render();

		//for fbo visualization
		CVK::State::getInstance()->setCamera( NULL);
		fbo.unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CVK::State::getInstance()->updateSceneSettings( grey, 0, white, 1, 10, 1);
		phongShader.update();
		screenFillingQuad->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}
	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

