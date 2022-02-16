#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "physics.h"


float red = 1;
float green = 0;
float blue = 0;
#define RATE 0.002
int steps = 0;

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

void rainbow() {
	if (steps >= 0 && steps < 500) {
		green += RATE;
	} else if (steps >= 500 && steps < 1000) {
		red -= RATE;
	} else if (steps >= 1000 && steps < 1500) {
		blue += RATE;
	} else if (steps >= 1500 && steps < 2000) {
		green -= RATE;
	} else if (steps >= 2000 && steps < 2500) {
		red += RATE;
	} else if (steps >= 2500 && steps < 3000) {
		blue -= RATE;
	}
}

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

void makeCircle() {
	float angle = PI / 180 * (360 / SEGMENTS);
	for (int j = 0; j < 3; j++)
		vertices.push_back(0.0f);
	for (int i = 0; i < SEGMENTS; i++) {
		std::vector<GLfloat> part;
		float currentAngle = angle * i;
		part.push_back(sin(currentAngle) * OBJ_RADIUS);
		part.push_back(cos(currentAngle) * OBJ_RADIUS);
		part.push_back(0.0f);
		vertices.insert(vertices.end(), part.begin(), part.end());
		indices.push_back(0);
		indices.push_back(i + 1);
		if (i + 1 == SEGMENTS)
			indices.push_back(1);
		else
			indices.push_back(i + 2);
	}
}

int main() {
	const int width = 700;
	const int height = width;

	makeCircle();

	glm::vec4 Color(blue, red, green, 1.0f);

	//Physics
	Physics ball;
	ball.mass = 1;
	ball.Pos.x = 0.67f;
	ball.Pos.y = 0.0f;
	ball.gravity.x = 0.0f;
	ball.gravity.y = -0.000098f;
	ball.velocity.x = -0.00f;
	ball.velocity.y = -0.005f;

	ball.generateMesh();
	ball.generateMeshPoints();
	ball.generateNormals();

	//Window
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (!window) {
		std::cout << "Window creation failed successfully\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	//Shaders
	const std::string vertexCode = 
	#include "ball_vertex.glsl"
	;
	const std::string fragmentCode = 
	#include "ball_fragment.glsl"
	;
	
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Buffers
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, size(vertices) * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size(indices) * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Uniforms
	GLint PosUniformLocation = glGetUniformLocation(shaderProgram, "Pos");
	GLint ColorUniformLocation = glGetUniformLocation(shaderProgram, "Color");

	while (!glfwWindowShouldClose(window)) {
		rainbow();
		Color.x = blue;
		Color.y = red;
		Color.z = green;

		ball.doPhysicsTick();
		ball.doPhysicsTick();
		ball.doPhysicsTick();
		ball.doPhysicsTick();

		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//Sending Uniforms
		glUniform2fv(PosUniformLocation, 1, &ball.Pos[0]);
		glUniform4fv(ColorUniformLocation, 1, &Color[0]);

		glDrawElements(GL_TRIANGLES, size(indices), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		steps++;
		if (steps == 3000)
			steps = 0;

		glfwPollEvents();
	}

	//Deleting things
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
