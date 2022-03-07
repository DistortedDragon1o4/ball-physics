#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

#include "physics.h"
#include "json.hpp"
#include "tinyfiledialogs.h"
#include "stb/stb_image.h"

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

glm::vec4 color(1.0f, 0.67f, 0.01f, 1.0f);


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

int hexToInt(std::string hex) {
	int digit1 = hex[1];
	int digit2 = hex[0];
	if (digit1 < 58)
		digit1 = digit1 - 48;
	else
		digit1 = digit1 - 87;
	if (digit2 < 58)
		digit2 = digit2 - 48;
	else
		digit2 = digit2 - 87;
	int final = digit1 + (digit2 * 16);
	return final;
}

void chooseColor() {
	unsigned char lResultRGB[3];
	const char* colorChar = tinyfd_colorChooser("Choose the color of the ball", "#ffab03", NULL, lResultRGB);

	if (!colorChar) {
		colorChar = "#ffab03";
	}

	std::string colorString = colorChar;
	std::string red = colorString.substr(1, 2);
	std::string green = colorString.substr(3, 2);
	std::string blue = colorString.substr(5, 2);

	color.x = hexToInt(red) / 255.0f;
	color.y = hexToInt(green) / 255.0f;
	color.z = hexToInt(blue) / 255.0f;
}

int main() {
	const int width = 700;
	const int height = width;

	makeCircle();

	//Physics
	Physics physics;
	int speed;

	//JSON Selector
	const char* fileType[1] = {"*.json"};
	const char* selection = tinyfd_openFileDialog("Select File", "~", 1, fileType, NULL, 0);

	//Loading JSON
	std::ifstream Properties;
	Properties.open(selection);

	if (!Properties) {
		tinyfd_messageBox("Warning", "No JSON file specified, using default settings", "ok", "warning", 1);
		Properties.close();
		
		chooseColor();

		speed = 2;
		physics.mass = 0.5f;
		physics.Pos.x = 0.0f;
		physics.Pos.y = 0.0f;
		physics.gravity.x = 0.0f;
		physics.gravity.y = -0.000098f;
		physics.velocity.x = 0.0f;
		physics.velocity.y = 0.0f;
	} else {
		std::stringstream s;
		s << Properties.rdbuf();
		std::string propertiesString = s.str();

		Properties.close();

		nlohmann::json properties = nlohmann::json::parse(propertiesString);

		speed = properties.value("speed", 2);

		//Putting JSON in physics
		physics.mass = properties.value("mass", 0.5f);
		physics.Pos.x = properties.value("PosX", 0.0f);
		physics.Pos.y = properties.value("PosY", 0.0f);
		physics.gravity.x = properties.value("gravityX", 0.0f);
		physics.gravity.y = properties.value("gravityY", -0.000098f);
		physics.velocity.x = properties.value("velocityX", 0.0f);
		physics.velocity.y = properties.value("velocityY", 0.0f);
		physics.enablePointNormals = properties.value("enablePointNormals", true);

		color.x = properties.value("red", 1.0f);
		color.y = properties.value("green", 0.67f);
		color.z = properties.value("blue", 0.01f);
	}

	physics.enablePointNormals = 0;

	physics.generateMesh();
	physics.generateMeshPoints();
	physics.generateNormals();

	int iwidth, iheight, numColch;

	//Window
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "2D Ball Physics", NULL, NULL);
	if (!window) {
		std::cout << "Window creation failed successfully\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLFWimage favicon[1];
	stbi_uc* faviconImg = stbi_load("favicon.png", &iwidth, &iheight, &numColch, 0);
	if (!faviconImg)
		std::cout << "Favicon loading failed successfully\n";
	favicon -> height = iheight;
	favicon -> width = iwidth;
	favicon[0].pixels = faviconImg;
	glfwSetWindowIcon(window, 1, favicon);

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

	//Env Shaders
	const std::string envVertexCode =
	#include "env_vertex.glsl"
	;
	const std::string envFragmentCode =
	#include "env_fragment.glsl"
	;

	const char* envVertexSource = envVertexCode.c_str();
	const char* envFragmentSource = envFragmentCode.c_str();

	GLuint envVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(envVertexShader, 1, &envVertexSource, NULL);
	glCompileShader(envVertexShader);

	GLuint envFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(envFragmentShader, 1, &envFragmentSource, NULL);
	glCompileShader(envFragmentShader);

	GLuint envShaderProgram = glCreateProgram();

	glAttachShader(envShaderProgram, envVertexShader);
	glAttachShader(envShaderProgram, envFragmentShader);
	glLinkProgram(envShaderProgram);

	glDeleteShader(envVertexShader);
	glDeleteShader(envFragmentShader);

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

	//Buffers
	GLuint envVAO, envVBO;
	glGenVertexArrays(1, &envVAO);
	glGenBuffers(1, &envVBO);

	glBindVertexArray(envVAO);
	glBindBuffer(GL_ARRAY_BUFFER, envVBO);

	glBufferData(GL_ARRAY_BUFFER, size(physics.mesh) * sizeof(float), &physics.mesh[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Uniforms
	GLint PosUniformLocation = glGetUniformLocation(shaderProgram, "Pos");
	GLint ColorUniformLocation = glGetUniformLocation(shaderProgram, "Color");

	while (!glfwWindowShouldClose(window)) {
		for (int i = 0; i < speed; i++)
			physics.doPhysicsTick();

		glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//Sending Uniforms
		glUniform2fv(PosUniformLocation, 1, &physics.Pos[0]);
		glUniform4fv(ColorUniformLocation, 1, &color[0]);

		glDrawElements(GL_TRIANGLES, size(indices), GL_UNSIGNED_INT, 0);

		glUseProgram(envShaderProgram);
		glBindVertexArray(envVAO);
		glDrawArrays(GL_LINE_LOOP, 0, size(physics.mesh) * 0.5f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Deleting things
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &envVAO);
	glDeleteBuffers(1, &envVBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(envShaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
