#include "windows.h"
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"


const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

static const char* vShader = "shaders/vShader.txt";
static const char* fShader = "shaders/fShader.txt";


void CreateObjects() 
{
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader );
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Init();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, .1f);

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth()/mainWindow.GetBufferHeight(), 0.1f, 100.f);

	// Loop unitl window closed
	while (!mainWindow.getShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(),deltaTime);
		camera.mouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());
		
		// Clear Window
		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0] -> GetViewLocation();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, glm::radians(curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.4f, .4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calcuateViewMatrix()));

		meshList[0]->RenderMesh();


		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		//model = glm::rotate(model, glm::radians(curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.4f, .4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}


