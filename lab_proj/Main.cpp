#include<filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;


std::string get_texture_path(std::string texture_name) {
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "/Resources/";
	return parentDir + texPath + texture_name;
}

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] = {
		/*	|		Coordinates			|			Colors				|		Axes		| */
				-0.5f, 0.0f,  0.5f,			0.83f, 0.70f, 0.44f,		0.0f, 0.0f,
				-0.5f, 0.0f, -0.5f,			0.83f, 0.70f, 0.44f,		5.0f, 0.0f,
				 0.5f, 0.0f, -0.5f,			0.83f, 0.70f, 0.44f,		0.0f, 0.0f,
				 0.5f, 0.0f,  0.5f,			0.83f, 0.70f, 0.44f,		5.0f, 0.0f,
				 0.0f, 0.8f,  0.0f,			0.92f, 0.86f, 0.76f,		2.5f, 5.0f
	};
	GLuint indices[] = {
						0, 1, 2,
						0, 2, 3,
						0, 1, 4,
						1, 2, 4,
						2, 3, 4,
						3, 0, 4
	};

	GLFWwindow* window = glfwCreateWindow(width, height, "proj_lab", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture brick_texture(get_texture_path("brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brick_texture.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.05f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		brick_texture.Bind();

		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	brick_texture.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}