#include <string.h>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

//for our kind mac users
#include <CVK_Framework/CVK_CompatibilityTools.h>



//our window pointer
GLFWwindow* window;


//with this function we load a shader source file 
//and pass it to the graphics driver
void loadShaderSource(int shaderHandle, const char* fileName) {
    std::string fileContent;
    std::string line;

    //open file and "parse" input
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (!file.eof()){
            getline (file, line);
            fileContent += line + "\n";
        }
        file.close();
        std::cout << "SUCCESS: Opened file " << fileName << std::endl;
    }
    else
        std::cout << "ERROR: Unable to open file " << fileName << std::endl;

    const char* source = fileContent.c_str();
    const GLint source_size = strlen(source);
    
    glShaderSource(shaderHandle, 1, &source, &source_size);
}



//with this function we get error messages in the console if an error occures 
//during shader compilation.
void checkShader(GLuint shaderHandle) {
    GLint status;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderHandle, infoLogLength, NULL, infoLog);

        std::cout << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    } else {
        std::cout << "SUCCESS: Shader compiled" << std::endl;
    }
}







int main() {
	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	window = glfwCreateWindow(800, 600, "Minimal Cube", 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();





    //================================================//
    //        we generate a shader program with       //
    //         a vertex and a fragment shader         //
    //================================================//

    //compile vertex shader
    GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    loadShaderSource(vertexShaderHandle, SHADERS_PATH "/Minimal/MVPTransform.vert");
    glCompileShader(vertexShaderHandle);
    checkShader(vertexShaderHandle);


    //compile fragment shader
    GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    loadShaderSource(fragmentShaderHandle, SHADERS_PATH "/Minimal/NormalColor.frag");
    glCompileShader(fragmentShaderHandle);
    checkShader(fragmentShaderHandle);


    //link shader programs
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShaderHandle);
    glAttachShader(programHandle, fragmentShaderHandle);
    glLinkProgram(programHandle);


    //get uniform location from program to upload matrices to during the render loop
    GLuint uniformModelHandle = glGetUniformLocation(programHandle, "modelMatrix");
    GLuint uniformViewHandle = glGetUniformLocation(programHandle, "viewMatrix");
    GLuint uniformProjectionHandle = glGetUniformLocation(programHandle, "projectionMatrix");





    //================================================//
    //        we generate a vertex array object       //
    //          and attach two buffers to it          //
    //================================================//

    //generate a vertex array object (VAO) for two attributes (positions and normals)
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //generate a vertex buffer object (VBO) for vertices and normals
    GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);



    //store the position attribute of all cube vertices into a vertex buffer object
    GLfloat positions[] = {
        // Front face
        -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,    1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,
        // Right face
         1.0, -1.0,  1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,
         1.0,  1.0, -1.0,    1.0,  1.0,  1.0,    1.0, -1.0,  1.0,
        // Back face
        -1.0, -1.0, -1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,
         1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,   -1.0, -1.0, -1.0,
        // Left face
        -1.0, -1.0,  1.0,   -1.0, -1.0, -1.0,   -1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,   -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,
        // Bottom face   
        -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,    1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,   -1.0, -1.0, -1.0,   -1.0, -1.0,  1.0,
        // Top Face
        -1.0,  1.0,  1.0,    1.0,  1.0,  1.0,    1.0,  1.0, -1.0,
         1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,   -1.0,  1.0,  1.0
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    //tell the VAO what we got here
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);



    //store the normal attribute of all cube vertices into a vertex buffer object
    GLfloat normals[] = {
        // Front face
         0.0,  0.0,  1.0,    0.0,  0.0,  1.0,    0.0,  0.0,  1.0,
         0.0,  0.0,  1.0,    0.0,  0.0,  1.0,    0.0,  0.0,  1.0,
        // Right face
         1.0,  0.0,  0.0,    1.0,  0.0,  0.0,    1.0,  0.0,  0.0,
         1.0,  0.0,  0.0,    1.0,  0.0,  0.0,    1.0,  0.0,  0.0,
        // Back face
         0.0,  0.0, -1.0,    0.0,  0.0, -1.0,    0.0,  0.0, -1.0,
         0.0,  0.0, -1.0,    0.0,  0.0, -1.0,    0.0,  0.0, -1.0,
        // Left face
        -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,
        -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,
        // Bottom face
         0.0, -1.0,  0.0,    0.0, -1.0,  0.0,    0.0, -1.0,  0.0,
         0.0, -1.0,  0.0,    0.0, -1.0,  0.0,    0.0, -1.0,  0.0,
        // Top face
         0.0,  1.0,  0.0,    0.0,  1.0,  0.0,    0.0,  1.0,  0.0,
         0.0,  1.0,  0.0,    0.0,  1.0,  0.0,    0.0,  1.0,  0.0
    };    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    //tell the VAO what we got here
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);






    float angle = 0.0;
    double oldTime = glfwGetTime();
    double deltaTime = 0;

    glClearColor(0, 0.1, 0.2, 1);


    //=================================================//
    //      ladies and gentlemen: the renderloop!      //
    //=================================================//

    while(!glfwWindowShouldClose( window)) {

    	//to get constant animation we need the time delta
    	deltaTime = glfwGetTime() - oldTime;
    	oldTime += deltaTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);



        //use the shader program...
        glUseProgram(programHandle);

        //... with this vertex array object
        glBindVertexArray(vertexArrayHandle);



        {
            //=================================================//
            //        we use glm to do the matrix stuff        //
            //=================================================//

            using namespace glm;
            
            //increase rotation angle by the time delta and clamp it to 2*pi
            angle = fmod(angle + (float)deltaTime, pi<float>() * 2.0f);

            //compute MODEL matrix and upload it to the uniform location at the gpu
            mat4 modelMatrix = rotate(mat4(1.0f), degrees(angle), vec3(1.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(uniformModelHandle, 1, GL_FALSE, value_ptr(modelMatrix));

            //compute VIEW matrix and upload it to the uniform location at the gpu
            mat4 viewMatrix = lookAt(vec3(0.0f, 2.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));    
            glUniformMatrix4fv(uniformViewHandle, 1, GL_FALSE, value_ptr(viewMatrix));

            //compute PROJECTION matrix and upload it to the uniform location at the gpu
            mat4 projectionMatrix = perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.f);
            glUniformMatrix4fv(uniformProjectionHandle, 1, GL_FALSE, value_ptr(projectionMatrix));
        }  



	    //=================================================//
	    //   this is where everything happens on the gpu   //
	    //=================================================//

        glDrawArrays(GL_TRIANGLES, 0, 3*6*2); //3 vertices a 2 triangles a 6 faces, 

        //show what's been drawn
		glfwSwapBuffers(window);
		glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
};